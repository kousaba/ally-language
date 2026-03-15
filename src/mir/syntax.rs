use crate::ast::base::{Type, Op};
use crate::base::span::Span;
use crate::sema::symbol::Symbol;

#[derive(Default, Debug)]
pub struct MirBody{
    pub blocks: Vec<BasicBlock>,
    pub locals: Vec<LocalDecl>,
}

#[derive(Debug)]
pub struct BasicBlock{
    pub statements: Vec<Statement>,
    pub terminator: Option<Terminator>,
}

#[derive(Debug)]
pub enum Statement{
    Assign(LocalId, RValue),
    StorageLive(LocalId),
    StorageDead(LocalId),
}

#[derive(Debug)]
pub enum Terminator{
    Goto(BlockId),
    Branch{ cond: Operand, then_b: BlockId, else_b: BlockId },
    Return,
}

#[derive(Debug)]
pub enum RValue{
    Use(Operand),
    BinaryOp(Op, Operand, Operand),
    Ref(LocalId, bool),
}

#[derive(Debug)]
pub enum Operand{
    Copy(LocalId),
    Move(LocalId),
    Constant(i64),
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct LocalId(pub usize);
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct BlockId(pub usize);

#[derive(Debug)]
pub struct LocalDecl{
    pub ty: Type,
    pub mutable: bool,
    pub name: Option<Symbol>,
    pub span: Span,
}


