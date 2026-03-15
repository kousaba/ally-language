pub mod token;

pub use token::{RawToken, AllyTokenKind, AllyToken};
use logos::Logos;
use crate::session::Session;
use crate::base::span::Span;

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
        let raw_result = self.inner.next()?;
        let logos_span = self.inner.span();
        let span = Span::new(logos_span.start as u32, logos_span.end as u32);
        let raw = raw_result.ok()?;
        let kind = match raw{
            RawToken::If => AllyTokenKind::If,
            RawToken::Else => AllyTokenKind::Else,
            RawToken::For => AllyTokenKind::For,
            RawToken::Let => AllyTokenKind::Let,
            RawToken::Mut => AllyTokenKind::Mut,
            RawToken::Fn => AllyTokenKind::Fn,
            RawToken::Ret => AllyTokenKind::Ret,
            RawToken::Eq => AllyTokenKind::Eq,
            RawToken::Ne => AllyTokenKind::Ne,
            RawToken::Lt => AllyTokenKind::Lt,
            RawToken::Gt => AllyTokenKind::Gt,
            RawToken::And => AllyTokenKind::And,
            RawToken::Or => AllyTokenKind::Or,
            RawToken::Assign => AllyTokenKind::Assign,
            RawToken::MoveAssign => AllyTokenKind::MoveAssign,
            RawToken::Plus => AllyTokenKind::Plus,
            RawToken::Minus => AllyTokenKind::Minus,
            RawToken::Asterisk => AllyTokenKind::Asterisk,
            RawToken::Slash => AllyTokenKind::Slash,
            RawToken::Dot => AllyTokenKind::Dot,
            RawToken::Ampersand => AllyTokenKind::Ampersand,
            RawToken::LBracket => AllyTokenKind::LBracket,
            RawToken::RBracket => AllyTokenKind::RBracket,
            RawToken::LParen => AllyTokenKind::LParen,
            RawToken::RParen => AllyTokenKind::RParen,
            RawToken::Colon => AllyTokenKind::Colon,
            RawToken::Semi => AllyTokenKind::Semi,
            RawToken::Int(n) => AllyTokenKind::Int(n),
            RawToken::Identifier(s) => AllyTokenKind::Identifier(sess.intern(&s)),
            RawToken::Unknown => {
                // TODO: Error出力
                return None;
            }
        };
        Some(AllyToken { kind, span} )
    }

    pub fn tokenize_all(&mut self, sess: &mut Session) -> Vec<AllyToken> {
        let mut tokens = Vec::new();
        while let Some(t) = self.next_token(sess){
            tokens.push(t);
        }
        let last_pos = self.inner.span().end as u32;
        tokens.push(AllyToken {
            kind: AllyTokenKind::EOF,
            span: Span::new(last_pos, last_pos),
        });
        tokens
    }
}
