use crate::lexer::{AllyToken, AllyTokenKind};
use crate::ast::base::{Op, Type, ExprInfo, Expr, StmtInfo, Stmt, Function, Module};
use crate::base::span::Span;
use crate::session::Session;
use lasso::Spur;

pub struct Parser<'a>{
    tokens: Vec<AllyToken>,
    pos: usize,
    sess: &'a Session,
}

#[derive(Clone, Copy)]
enum ExprPrecedence{
    None = 0,
    Assignment = 10, // =
    Or = 20, // ||
    And = 30, // &&
    Comparison = 40, // == != < > <= >= 
    Sum = 50, // + -
    Product = 60, // * /
    Prefix = 70, // ! - (Unary)
    Call = 80, // . () []
}

impl<'a> Parser<'a>{
    pub fn new(tokens: Vec<AllyToken>, session: &'a Session) -> Self{
        Self { tokens, pos: 0, sess: session}
    }
    pub fn parse_module(&mut self) -> Module{
        let mut module = Module::new();
        loop{
            let func = self.next_function();
            match func{
                Some(func) => module.add_function(func),
                None => break,
            }
        }
        module
    }
    fn peek(&self) -> Option<AllyToken>{
        self.tokens.get(self.pos).copied()
    }
    fn consume(&mut self) -> AllyToken{
        let tok = self.peek().unwrap();
        self.pos += 1;
        tok
    }
    fn check(&self, token_type: AllyTokenKind) -> bool {
        self.peek().is_some_and(|x| x.kind == token_type)
    }
    fn check_consume(&mut self, token_type: AllyTokenKind) -> bool {
        if self.check(token_type){
            self.consume();
            return true;
        }
        false
    }
    fn next_function(&mut self) -> Option<Function>{
        if !self.check(AllyTokenKind::Fn){
            return None;
        }
        self.consume();
        let fn_name_id = match self.consume().kind{
            AllyTokenKind::Identifier(function_id) => function_id,
            _ => return None,
        };
        self.consume(); // (
        self.consume(); // )
        let block = self.parse_stmt();
        Some(Function::new(fn_name_id, block?))
    }
    fn parse_stmt(&mut self) -> Option<Stmt>{
        let token = self.peek()?;
        let res = match token.kind {
            AllyTokenKind::If => Some(self.parse_stmt_if()),
            AllyTokenKind::Let => Some(self.parse_stmt_let()),
            AllyTokenKind::Ret => Some(self.parse_stmt_ret()),
            AllyTokenKind::LBracket => Some(self.parse_stmt_block()),
            _ => None?,
        };
        if let Some(ref stmt) = res{
            match stmt.info{
                StmtInfo::Let { .. } | StmtInfo::Ret(..) => {
                    if !self.check_consume(AllyTokenKind::Semi){
                        panic!("Expected ';' after statement");
                    }
                }
                _ => {}
            }
        }
        res
    }
    fn parse_stmt_if(&mut self) -> Stmt{
        let start_token = self.consume();
        let start_span = self.get_span(&start_token);
        let condition = self.parse_expr();
        let then_block = self.parse_stmt();
        if self.check_consume(AllyTokenKind::Else){
            let else_block = self.parse_stmt();
            let span = start_span.merge(else_block.as_ref().unwrap().span);
            Stmt::new(StmtInfo::If{
                cond: condition,
                then_b: Box::new(then_block.unwrap()),
                else_b: Box::new(else_block.unwrap()),
            }, span)
        }else{
            let span = start_span.merge(then_block.as_ref().unwrap().span);
            Stmt::new(StmtInfo::If{
                cond: condition,
                then_b: Box::new(then_block.unwrap()),
                else_b: Box::new(Stmt::new(StmtInfo::Unknown, span)),
            }, span)
        }
    }
    fn parse_stmt_let(&mut self) -> Stmt{
        let start_token = self.consume();
        let start_span = self.get_span(&start_token);
        let is_mutable = self.check_consume(AllyTokenKind::Mut);
        let var_name = match self.consume().kind{
            AllyTokenKind::Identifier(name) => name,
            _ => unreachable!(),
        };
        let mut vartype = Type::Unknown;
        if self.check_consume(AllyTokenKind::Colon){
            let type_name_id = match self.consume().kind{
                AllyTokenKind::Identifier(name_id) => name_id,
                _ => unreachable!(),
            };
            vartype = self.id_to_type(type_name_id);
        }
        let mut end_span = start_span;
        let mut value = Expr::new(ExprInfo::Unknown, start_span);
        if self.check_consume(AllyTokenKind::Assign) {
            value = self.parse_expr();
            end_span = value.span;
        }
        Stmt::new(
            StmtInfo::Let { name: var_name, value, mutable: is_mutable, var_type: vartype },
            start_span.merge(end_span)
        )
    }
    fn id_to_type(&self, id: Spur) -> Type{
        let type_str = self.sess.lookup(id);
        match type_str{
            "int" => Type::Int,
            "i32" => Type::I32,
            "uint" => Type::Uint,
            "u32" => Type::U32,
            "f64" => Type::F64,
            "f32" => Type::F32,
            "chr" => Type::Chr,
            _ => Type::Unknown,
        }
    }
    fn parse_stmt_ret(&mut self) -> Stmt{
        let start_token = self.consume();
        let start_span = self.get_span(&start_token);
        self.consume();
        let expr = self.parse_expr();
        let end_span = expr.span;
        Stmt::new(
            StmtInfo::Ret(expr),
            start_span.merge(end_span)
        )
    }
    fn parse_stmt_block(&mut self) -> Stmt{
        let l_bracket = self.consume();
        let start_span = self.get_span(&l_bracket);
        let mut stmts = Vec::new();
        while !self.check(AllyTokenKind::RBracket) {
            if let Some(stmt) = self.parse_stmt(){
                stmts.push(stmt);
            }else{
                break;
            }
        }
        let r_bracket = self.consume();
        let span = start_span.merge(self.get_span(&r_bracket));
        Stmt::new(StmtInfo::Block(stmts), span)
    }
    fn parse_expr(&mut self) -> Expr{
        self.parse_expr_precedence(ExprPrecedence::None)
    }
    fn parse_expr_precedence(&mut self, min_precedence: ExprPrecedence) -> Expr{
        let mut left = self.parse_prefix();
        while let Some(token) = self.peek(){
            let next_precedence = self.get_precedence(token);
            if(next_precedence as u8) <= (min_precedence as u8){
                break;
            }
            self.consume();
            left = self.parse_infix(left, next_precedence);
        }
        left
    }
    fn parse_prefix(&mut self) -> Expr{
        let token = self.consume();
        let start_span = self.get_span(&token);
        match token.kind {
            AllyTokenKind::Identifier(id) => Expr::new(ExprInfo::Variable(id), start_span),
            AllyTokenKind::Int(val) => Expr::new(ExprInfo::Number(val), start_span),
            AllyTokenKind::Minus => {
                let right = self.parse_expr_precedence(ExprPrecedence::Prefix);
                let right_span = right.span;
                Expr::new(
                    ExprInfo::UnaryOp{
                        op: Op::Neg,
                        expr: Box::new(right),
                    },
                    start_span.merge(right_span)
                )
            }
            AllyTokenKind::LParen => {
                let expr = self.parse_expr();
                let r_paren = self.consume();
                let span = start_span.merge(self.get_span(&r_paren));
                Expr::new(expr.info, span)
            }
            AllyTokenKind::Ampersand => {
                let is_mut = self.check_consume(AllyTokenKind::Mut);
                let right = self.parse_expr_precedence(ExprPrecedence::Prefix);
                let span = start_span.merge(right.span);
                Expr::new(ExprInfo::Borrow { is_mut, expr: Box::new(right) }, span)
            }
            AllyTokenKind::Asterisk => {
                let right = self.parse_expr_precedence(ExprPrecedence::Prefix);
                let span = start_span.merge(right.span);
                Expr::new(ExprInfo::Dereference(Box::new(right)), span)
            }
            _ => unreachable!()
        }
    }
    fn parse_infix(&mut self, left: Expr, precedence: ExprPrecedence) -> Expr{
        let token = self.tokens[self.pos - 1];
        match token.kind{
            AllyTokenKind::Plus | AllyTokenKind::Minus | AllyTokenKind::Asterisk | AllyTokenKind::Slash |
            AllyTokenKind::Eq | AllyTokenKind::Ne | AllyTokenKind::Lt | AllyTokenKind::Gt |
            AllyTokenKind::And | AllyTokenKind::Or => {
                let right = self.parse_expr_precedence(precedence);
                let span = left.span.merge(right.span);
                Expr::new(ExprInfo::BinaryOp { op: self.token_to_op(token).unwrap(), lhs: Box::new(left), rhs: Box::new(right) }, span)
            }
            AllyTokenKind::LParen => {
                // self.parse_call(left)
                todo!()
            }
            _ => left,
        }
    }
    fn token_to_op(&self, token_type: AllyToken) -> Option<Op>{
        match token_type.kind {
            AllyTokenKind::Eq => Some(Op::EQ),
            AllyTokenKind::Ne => Some(Op::NEQ),
            AllyTokenKind::Lt => Some(Op::LT),
            AllyTokenKind::Gt => Some(Op::GT),
            AllyTokenKind::And => Some(Op::And),
            AllyTokenKind::Or => Some(Op::Or),
            AllyTokenKind::Plus => Some(Op::Add),
            AllyTokenKind::Minus => Some(Op::Sub),
            AllyTokenKind::Asterisk => Some(Op::Mul),
            AllyTokenKind::Slash => Some(Op::Div),
            _ => unreachable!(),
        }
    }
    fn get_precedence(&self, token: AllyToken) -> ExprPrecedence {
        match token.kind {
            AllyTokenKind::Assign | AllyTokenKind::MoveAssign => ExprPrecedence::Assignment,
            AllyTokenKind::Or => ExprPrecedence::Or,
            AllyTokenKind::And => ExprPrecedence::And,
            AllyTokenKind::Eq | AllyTokenKind::Ne | AllyTokenKind::Lt | AllyTokenKind::Gt => ExprPrecedence::Comparison,
            AllyTokenKind::Plus | AllyTokenKind::Minus => ExprPrecedence::Sum,
            AllyTokenKind::Asterisk | AllyTokenKind::Slash => ExprPrecedence::Product,
            AllyTokenKind::LParen | AllyTokenKind::Dot => ExprPrecedence::Call,
            _ => ExprPrecedence::None,
        }
    }
    fn get_span(&self, token: &AllyToken) -> Span{
        token.span
    }
}
