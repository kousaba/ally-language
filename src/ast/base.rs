use crate::session::Symbol;
use crate::base::span::Span;

#[derive(Debug)]
pub enum Op{
    Neg,
    Not,
    Add,
    Sub,
    Mul,
    Div,
    And,
    Or,
    LT,
    GT,
    EQ,
    NEQ,
}

#[derive(Debug)]
pub enum Type{
    Int,
    I32,
    Uint,
    U32,
    F64,
    F32,
    Chr,
    Void,
    Unknown,
}

#[derive(Debug)]
pub enum ExprInfo{
    Number(i64),
    Variable(Symbol),
    UnaryOp{ op: Op, expr: Box<Expr> },
    BinaryOp{ op: Op, lhs: Box<Expr>, rhs: Box<Expr> },
    Borrow { is_mut: bool, expr: Box<Expr> },
    Dereference(Box<Expr>),
    Unknown,
}

#[derive(Debug)]
pub struct Expr{
    pub info: ExprInfo,
    pub expr_type: Type,
    pub span: Span,
}

impl Expr{
    pub fn new(info: ExprInfo, span: Span) -> Self{
        Self{
            info,
            expr_type: Type::Unknown,
            span,
        }
    }
}

#[derive(Debug)]
pub enum StmtInfo{
    Ret(Expr),
    Let{ name: Symbol, value: Expr, mutable: bool, var_type: Type },
    Block(Vec<Stmt>),
    If{ cond: Expr, then_b: Box<Stmt>, else_b: Box<Stmt> },
    Unknown,
}

#[derive(Debug)]
pub struct Stmt{
    pub info: StmtInfo,
    pub span: Span,
}

impl Stmt{
    pub fn new(info: StmtInfo, span: Span) -> Self{
        Self { info, span }
    }
}

#[derive(Debug)]
pub struct Function{
    pub name: Symbol,
    pub body: Stmt,
}

impl Function{
    pub fn new(function_name: Symbol, function_body: Stmt) -> Function{
        Self{
            name: function_name,
            body: function_body,
        }
    }
}

#[derive(Debug)]
pub enum TopLevel{
    Fn{ name: Symbol, block: Stmt },
}

#[derive(Debug, Default)]
pub struct Module {
    pub functions: Vec<Function>,
}

impl Module {
    pub fn new() -> Module{
        Module{
            functions: Vec::new()
        } 
    }
    pub fn add_function(&mut self, function: Function){
        self.functions.push(function);
    }
}
