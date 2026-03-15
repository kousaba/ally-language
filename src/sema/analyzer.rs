use crate::ast::base::{Module, Function, Type, Stmt, Expr, StmtInfo, ExprInfo};
use crate::sema::symbol::Symbol;
use crate::sema::scope::{ScopeStack, VariableInfo};
use std::collections::HashMap;

pub struct FunctionSignature{
    pub param_types: Vec<Type>,
    pub ret_type: Type,
}

#[derive(Default)]
pub struct Analyzer{
    functions: HashMap<Symbol, FunctionSignature>,
}

impl Analyzer{
    pub fn analyze(&mut self, module: &mut Module){
        // パス1
        for func in &module.functions{
            self.register_function(func);
        }
        // パス2
        for func in &mut module.functions{
            self.check_function(func);
        }
    }
    fn register_function(&mut self, func: &Function){
        /*
         * let signature = FunctionSignature{
         *     param_types: func.params.iter().map(|p| p.ty.clone()).collect(),
         *     ret_type: func.ret_type_clone(),
         * };
         */
        let signature = FunctionSignature{
            param_types: vec![],
            ret_type: Type::Int,
        };
        self.functions.insert(Symbol(func.name), signature);
    }
    fn check_function(&self, func: &mut Function){
        let mut scopes = ScopeStack::new();
        let sig = self.functions.get(&Symbol(func.name)).unwrap();
        // todo: 引数をスコープに登録
        self.check_stmt(&mut func.body, &mut scopes);
    }
    fn check_expr(&self, expr: &mut Expr, scopes: &ScopeStack) -> Type{
        match &mut expr.info{
            ExprInfo::Number(_) => {
                expr.expr_type = Type::Int;
            }
            ExprInfo::Variable(symbol) => {
                let info = scopes.lookup(Symbol(*symbol)).expect("Undefined variable");
                expr.expr_type = info.ty.clone();
            }
            ExprInfo::BinaryOp{op, lhs, rhs} => {
                let lhs_type = self.check_expr(lhs, scopes);
                let rhs_type = self.check_expr(rhs, scopes);
                if lhs_type != rhs_type{
                    // 同じ型ではない
                    dbg!(lhs_type);
                    dbg!(rhs_type);
                    todo!()
                }
                expr.expr_type = lhs_type;
                // todo: 条件演算子ならbool
            }
            ExprInfo::UnaryOp{op, expr} => {
                // 中身のまま
                expr.expr_type = self.check_expr(expr, scopes);
            }
            ExprInfo::Borrow{is_mut, expr} => {
                expr.expr_type = Type::Ptr(Box::new(self.check_expr(expr, scopes)));
            }
            ExprInfo::Dereference(expr) => {
                if let Type::Ptr(inner) | Type::MutPtr(inner) = &expr.expr_type{
                }else{
                    // ポインタではない
                    todo!()
                }
            }
            ExprInfo::Unknown => {
                expr.expr_type = Type::Unknown;
            }
        }
        expr.expr_type.clone()
    }
    fn check_stmt(&self, stmt: &mut Stmt, scopes: &mut ScopeStack){ 
        match &mut stmt.info{
            StmtInfo::Ret(expr) => {
                // 関数の返り値と型が同じか判定
                let ret_type = self.check_expr(expr, scopes);
                /*if ret_type != func.ret_type{
                    // 関数の返り値と一致しない
                    todo!();
                }*/
            }
            StmtInfo::Let{name, value, mutable, var_type} => {
                if scopes.lookup(Symbol(*name)).is_some(){
                    // 名前のかぶりがある
                }
                let expr_type = self.check_expr(value, scopes);
                if *var_type == Type::Unknown{
                    *var_type = expr_type.clone();
                }
                scopes.define(Symbol(*name), VariableInfo{ty: var_type.clone(), is_mutable: *mutable});
                if expr_type != *var_type && *var_type != Type::Unknown{
                    // var_typeとvalueの型が不一致
                    todo!()
                }
            }
            StmtInfo::Block(stmts) => {
                scopes.push();
                // すべてのstmtをチェック
                for in_stmt in stmts{
                    self.check_stmt(in_stmt, scopes);
                }
                scopes.pop();
            }
            StmtInfo::If{cond, then_b, else_b} => {
                let cond_ty = self.check_expr(cond, scopes);
                if cond_ty != Type::Bool{
                    // condがboolではない
                    todo!()
                }
                self.check_stmt(then_b, scopes);
                self.check_stmt(else_b, scopes);
            }
            StmtInfo::Unknown => {}
        }
    }

}
