use crate::session::Session;
use crate::lexer::{Lexer, AllyToken};
use crate::ast::base::Module;
use crate::parser::Parser;
use std::time::Instant;
use anyhow::Result;

pub fn compile(sess: &mut Session, source: &str) -> Result<()> {
    let mut start = Instant::now();
    let token = lexer(sess, source);
    let lexer_duration = start.elapsed();
    start = Instant::now();
    let ast = parser(sess, token);
    let parser_duration = start.elapsed();
    dbg!(ast);
    println!("Lexer: {:?}ms", lexer_duration.as_millis());
    println!("Parser: {:?}ms", parser_duration.as_millis());
    Ok(())
}

fn lexer(sess: &mut Session, source: &str) -> Vec<AllyToken>{
    let mut lexer = Lexer::new(source);
    let tokens = lexer.tokenize_all(sess);

    for token in &tokens{
        println!("Token: {token:?}");
    }
    
    tokens
}

fn parser(sess: &mut Session, tokens: Vec<AllyToken>) -> Module{
    let mut parser = Parser::new(tokens, sess);
    parser.parse_module()
}
