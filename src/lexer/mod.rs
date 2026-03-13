pub mod token;

pub use token::{RawToken, AllyToken};
use logos::Logos;
use crate::session::Session;

pub struct Lexer<'a> {
    inner: logos::Lexer<'a, RawToken>,
}

impl<'a> Lexer<'a> {
    pub fn new(source: &'a str) -> Self{
        Self{
            inner: RawToken::lexer(source)
        }
    }

    pub fn next_token(&mut self, sess: &mut Session) -> Option<AllyToken> {
        let raw = self.inner.next()?.ok()?;
        let token = match raw {
            RawToken::If => AllyToken::If,
            RawToken::Else => AllyToken::Else,
            RawToken::For => AllyToken::For,
            RawToken::Let => AllyToken::Let,
            RawToken::Mut => AllyToken::Mut,
            RawToken::Fn => AllyToken::Fn,
            RawToken::Ret => AllyToken::Ret,
            RawToken::Eq => AllyToken::Eq,
            RawToken::Ne => AllyToken::Ne,
            RawToken::Lt => AllyToken::Lt,
            RawToken::Gt => AllyToken::Gt,
            RawToken::And => AllyToken::And,
            RawToken::Or => AllyToken::Or,
            RawToken::Assign => AllyToken::Assign,
            RawToken::Plus => AllyToken::Plus,
            RawToken::Minus => AllyToken::Minus,
            RawToken::Asterisk => AllyToken::Asterisk,
            RawToken::Slash => AllyToken::Slash,
            RawToken::Dot => AllyToken::Dot,
            RawToken::Ampersand => AllyToken::Ampersand,
            RawToken::LBracket => AllyToken::LBracket,
            RawToken::RBracket => AllyToken::RBracket,
            RawToken::LParen => AllyToken::LParen,
            RawToken::RParen => AllyToken::RParen,
            RawToken::Semi => AllyToken::Semi,
            RawToken::Int(n) => AllyToken::Int(n),
            RawToken::Identifier(s) => AllyToken::Identifier(sess.intern(&s)),
            RawToken::Unknown => {
                // TODO: Error出力
                return None;
            }
        };
        Some(token)
    }

    pub fn tokenize_all(&mut self, sess: &mut Session) -> Vec<AllyToken> {
        let mut tokens = Vec::new();
        while let Some(t) = self.next_token(sess) {
            tokens.push(t);
        }
        tokens.push(AllyToken::EOF);
        tokens
    }

}
