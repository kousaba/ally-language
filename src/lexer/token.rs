use logos::Logos;
use crate::session::Symbol;

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
pub enum AllyToken{
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
    Semi,
    Identifier(Symbol),
    Int(i64),
    EOF,
}
