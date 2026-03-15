use logos::Logos;
use crate::session::Symbol;
use crate::base::span::Span;

#[derive(Logos, Debug, Clone)]
pub enum RawToken {
    #[token("if")]
    If,
    #[token("else")]
    Else,
    #[token("for")]
    For,
    #[token("let")]
    Let,
    #[token("mut")]
    Mut,
    #[token("fn")]
    Fn,
    #[token("ret")]
    Ret,
    
    #[token("==")]
    Eq,
    #[token("!=")]
    Ne,
    #[token("<")]
    Lt,
    #[token(">")]
    Gt,
    #[token("&&")]
    And,
    #[token("||")]
    Or,
    #[token("=")]
    Assign,
    #[token("<-")]
    MoveAssign,
    #[token("+")]
    Plus,
    #[token("-")]
    Minus,
    #[token("*")]
    Asterisk,
    #[token("/")]
    Slash,
    #[token(".")]
    Dot,
    #[token("&")]
    Ampersand,
    #[token("{")]
    LBracket,
    #[token("}")]
    RBracket,
    #[token("(")]
    LParen,
    #[token(")")]
    RParen,
    #[token(":")]
    Colon,
    #[token(";")]
    Semi,

    #[regex("[a-zA-Z_][a-zA-Z0-9_]*", |lex| lex.slice().to_string())]
    Identifier(String),

    #[regex("[0-9]+", |lex| lex.slice().parse::<i64>().unwrap())]
    Int(i64),

    #[regex(r"[ \t\n\f]+", logos::skip)]
    Unknown,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum AllyTokenKind{
    If,
    Else,
    For,
    Let,
    Mut,
    Ret,
    Eq,
    Ne,
    Lt,
    Gt,
    And,
    Or,
    Assign,
    MoveAssign,
    Plus,
    Minus,
    Asterisk,
    Slash,
    Dot,
    Ampersand,
    LBracket,
    RBracket,
    LParen,
    RParen,
    Fn,
    Colon,
    Semi,
    Identifier(Symbol),
    Int(i64),
    EOF,
}

#[derive(Debug, Clone, Copy)]
pub struct AllyToken{
    pub kind: AllyTokenKind,
    pub span: Span,
}

impl AllyToken{
    pub fn new(kind: AllyTokenKind, span: Span) -> Self{
        Self {kind, span}
    }
}
