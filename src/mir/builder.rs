use crate::mir::syntax::*;
use crate::session::Session;
use crate::ast::*;
use crate::ast::base::{StmtInfo, ExprInfo};
use std::collections::HashMap;
use crate::sema::symbol::Symbol;
use crate::base::span::Span;

pub struct MirBuilder{
    locals: Vec<LocalDecl>,
    blocks: Vec<BasicBlock>,
    symbol_map: HashMap<Symbol, LocalId>,
    current_block: BlockId,
}

impl MirBuilder{
    pub fn new() -> Self{
        let mut builder = Self{
            blocks: Vec::new(),
            locals: Vec::new(),
            symbol_map: HashMap::new(),
            current_block: BlockId(0),
        };
        builder.push_local(base::Type::Int, false, None, Span::new(1, 1)); // 戻り値用
        builder.new_block();
        builder
    }
    pub fn build_func(mut self, func: base::Function) -> MirBody{
        self.lower_stmt(func.body);
        if !self.current_block_terminated() {
            self.terminate(Terminator::Return);
        }
        MirBody{
            blocks: self.blocks,
            locals: self.locals,
        }
    }
    fn new_block(&mut self) -> BlockId{
        let id = BlockId(self.blocks.len());
        self.blocks.push(BasicBlock{
            statements: Vec::new(),
            terminator: None,
        });
        id
    }
    fn push_local(&mut self, ty: base::Type, mutable: bool, name: Option<Symbol>, span: Span) -> LocalId{
        let id = LocalId(self.locals.len());
        self.locals.push(LocalDecl{ty, mutable, name, span,});
        id
    }
    fn push_stmt(&mut self, stmt: Statement){
        self.blocks[self.current_block.0].statements.push(stmt);
    }
    fn terminate(&mut self, term: Terminator){
        self.blocks[self.current_block.0].terminator = Some(term);
    }
    fn current_block_terminated(&self) -> bool{
        self.blocks[self.current_block.0].terminator.is_some()
    }

    fn lower_stmt(&mut self, stmt: base::Stmt){
        let span = stmt.span;
        match stmt.info{
            StmtInfo::Let{name, value, mutable, var_type} => {
                let src = self.lower_expr(value);
                let local_id = self.push_local(var_type, mutable, Some(Symbol(name)), span);
                self.symbol_map.insert(Symbol(name), local_id);
                self.push_stmt(Statement::Assign(local_id, RValue::Use(src)));
            }
            StmtInfo::Ret(expr) => {
                let src = self.lower_expr(expr);
                self.push_stmt(Statement::Assign(LocalId(0), RValue::Use(src)));
                self.terminate(Terminator::Return);
            }
            StmtInfo::Block(stmts) => {
                for s in stmts{
                    self.lower_stmt(s);
                }
            }
            StmtInfo::If{cond, then_b, else_b} => {
                let cond_op = self.lower_expr(cond);
                let then_id = self.new_block();
                let else_id = self.new_block();
                let merge_id = self.new_block();

                self.terminate(Terminator::Branch{
                    cond: cond_op,
                    then_b: then_id,
                    else_b: else_id,
                });
                self.current_block = then_id;
                self.lower_stmt(*then_b);
                if !self.current_block_terminated(){
                    self.terminate(Terminator::Goto(merge_id));
                }
                self.current_block = else_id;
                self.lower_stmt(*else_b);
                if !self.current_block_terminated(){
                    self.terminate(Terminator::Goto(merge_id));
                }

                self.current_block = merge_id;
            }
            StmtInfo::Unknown => ()
        }
    }

    fn lower_expr(&mut self, expr: base::Expr) -> Operand{
        let span = expr.span;
        let ty = expr.expr_type;
        match expr.info{
            ExprInfo::Number(N) => Operand::Constant(N),
            ExprInfo::Variable(sym) => {
                let local_id = self.symbol_map.get(&Symbol(sym)).expect("Undefined symbol in MIR");
                Operand::Copy(*local_id) // todo: Move
            }
            ExprInfo::BinaryOp {op, lhs, rhs} => {
                let l_op = self.lower_expr(*lhs);
                let r_op = self.lower_expr(*rhs);
                let temp = self.push_local(ty, false, None, span);
                self.push_stmt(Statement::Assign(
                        temp,
                        RValue::BinaryOp(op, l_op, r_op)
                    ));
                Operand::Move(temp)
            }
            ExprInfo::Borrow{is_mut, expr: target_expr} => {
                if let ExprInfo::Variable(sym) = target_expr.info{
                    let target_id = *self.symbol_map.get(&Symbol(sym)).unwrap();
                    let temp = self.push_local(ty, false, None, span);
                    self.push_stmt(Statement::Assign(
                        temp,
                        RValue::Ref(target_id, is_mut)
                    ));
                    Operand::Move(temp)
                }else{
                    panic!("Only variables can be borrowed for now");
                }
            }
            _ => todo!()
        }
    }
}
