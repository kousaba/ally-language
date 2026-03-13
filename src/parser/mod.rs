use crate::lexer::AllyToken;
use crate::ast::base::{Op, Expr, Stmt, Function, Module};

pub struct Parser{
    tokens: Vec<AllyToken>,
    pos: usize,
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

impl Parser {
    pub fn new(tokens: Vec<AllyToken>) -> Self{
        Self { tokens, pos: 0}
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
    fn check(&self, token_type: AllyToken) -> bool {
        self.peek().is_some() && self.peek().unwrap() == token_type
    }
    fn check_consume(&mut self, token_type: AllyToken) -> bool {
        if self.check(token_type){
            self.consume();
            return true;
        }
        false
    }
    fn next_function(&mut self) -> Option<Function>{
        match self.peek(){
            Some(AllyToken::Fn) => self.consume(),
            _ => return None,
        };
        let fn_name_id = match self.consume(){
            AllyToken::Identifier(function_id) => function_id,
            _ => return None,
        };
        self.consume(); // (
        self.consume(); // )
        let block = self.parse_stmt();
        Some(Function::new(fn_name_id, block?))
    }
    fn parse_stmt(&mut self) -> Option<Stmt>{
        let token = self.peek()?;
        let res = match token {
            AllyToken::If => Some(self.parse_stmt_if()),
            AllyToken::Let => Some(self.parse_stmt_let()),
            AllyToken::Ret => Some(self.parse_stmt_ret()),
            AllyToken::LBracket => Some(self.parse_stmt_block()),
            _ => None?,
        };
        if let Some(ref stmt) = res{
            match stmt{
                Stmt::Let { .. } | Stmt::Ret(..) => {
                    if !self.check_consume(AllyToken::Semi){
                        panic!("Expected ';' after statement");
                    }
                }
                _ => {}
            }
        }
        res
    }
    fn parse_stmt_if(&mut self) -> Stmt{
        self.consume();
        let condition = self.parse_expr();
        let then_block = self.parse_stmt();
        if self.check_consume(AllyToken::Else){
            let else_block = self.parse_stmt();
            Stmt::If{
                cond: condition,
                then_b: Box::new(then_block.unwrap()),
                else_b: Box::new(else_block.unwrap()),
            }
        }else{
            Stmt::If{
                cond: condition,
                then_b: Box::new(then_block.unwrap()),
                else_b: Box::new(Stmt::Unknown),
            }
        }
    }
    fn parse_stmt_let(&mut self) -> Stmt{
        self.consume();
        let is_mutable = self.check_consume(AllyToken::Mut);
        let var_name = match self.consume(){
            AllyToken::Identifier(name) => name,
            _ => unreachable!(),
        };
        if self.check_consume(AllyToken::Assign) {
            let init_value = self.parse_expr();
            Stmt::Let{
                name: var_name,
                value: init_value,
                mutable: is_mutable,
            }
        }else{
            Stmt::Let{
                name: var_name,
                value: Expr::Unknown,
                mutable: is_mutable,
            }
        }
    }
    fn parse_stmt_ret(&mut self) -> Stmt{
        self.consume(); // ret keyword
        self.consume(); // = todo: <-実装
        let expr = self.parse_expr();
        Stmt::Ret(expr)
    }
    fn parse_stmt_block(&mut self) -> Stmt{
        self.consume();
        let mut stmts = Vec::new();
        while !self.check_consume(AllyToken::RBracket) {
            if let Some(stmt) = self.parse_stmt(){
                stmts.push(stmt);
            }else{
                let bad_token = self.peek();
                panic!("Unexpected token in block: {:?}", bad_token);
            }
        }
        self.consume(); // } 
        Stmt::Block(stmts)
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
        match token {
            AllyToken::Identifier(id) => Expr::Variable(id),
            AllyToken::Int(val) => Expr::Number(val),
            AllyToken::Minus => {
                let right = self.parse_expr_precedence(ExprPrecedence::Prefix);
                Expr::UnaryOp{
                    op: Op::Neg,
                    expr: Box::new(right),
                }
            }
            AllyToken::LParen => {
                let expr = self.parse_expr_precedence(ExprPrecedence::None);
                self.check_consume(AllyToken::RParen);
                expr
            }
            AllyToken::Ampersand => {
                let is_mut = self.check_consume(AllyToken::Mut);
                let right = self.parse_expr_precedence(ExprPrecedence::Prefix);
                Expr::Borrow{
                    is_mut,
                    expr: Box::new(right),
                }
            }
            AllyToken::Asterisk => {
                let right = self.parse_expr_precedence(ExprPrecedence::Prefix);
                Expr::Dereference(Box::new(right))
            }
            _ => unreachable!()
        }
    }
    fn parse_infix(&mut self, left: Expr, precedence: ExprPrecedence) -> Expr{
        let token = self.tokens[self.pos - 1];
        match token{
            AllyToken::Plus | AllyToken::Minus | AllyToken::Asterisk | AllyToken::Slash |
            AllyToken::Eq | AllyToken::Ne | AllyToken::Lt | AllyToken::Gt |
            AllyToken::And | AllyToken::Or => {
                let right = self.parse_expr_precedence(precedence);
                Expr::BinaryOp{
                    op: self.token_to_op(token).unwrap(),
                    lhs: Box::new(left),
                    rhs: Box::new(right),
                }
            }
            AllyToken::LParen => {
                // self.parse_call(left)
                todo!()
            }
            _ => left,
        }
    }
    fn token_to_op(&self, token_type: AllyToken) -> Option<Op>{
        match token_type {
            AllyToken::Eq => Some(Op::EQ),
            AllyToken::Ne => Some(Op::NEQ),
            AllyToken::Lt => Some(Op::LT),
            AllyToken::Gt => Some(Op::GT),
            AllyToken::And => Some(Op::And),
            AllyToken::Or => Some(Op::Or),
            AllyToken::Plus => Some(Op::Add),
            AllyToken::Minus => Some(Op::Sub),
            AllyToken::Asterisk => Some(Op::Mul),
            AllyToken::Slash => Some(Op::Div),
            _ => unreachable!(),
        }
    }
    fn get_precedence(&self, token: AllyToken) -> ExprPrecedence {
        match token {
            AllyToken::Assign => ExprPrecedence::Assignment,
            AllyToken::Or => ExprPrecedence::Or,
            AllyToken::And => ExprPrecedence::And,
            AllyToken::Eq | AllyToken::Ne | AllyToken::Lt | AllyToken::Gt => ExprPrecedence::Comparison,
            AllyToken::Plus | AllyToken::Minus => ExprPrecedence::Sum,
            AllyToken::Asterisk | AllyToken::Slash => ExprPrecedence::Product,
            AllyToken::LParen | AllyToken::Dot | AllyToken::LBracket => ExprPrecedence::Call,
            _ => ExprPrecedence::None,
        }
    }
}
