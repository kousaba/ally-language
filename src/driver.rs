use crate::session::Session;
use crate::lexer::{Lexer, AllyToken};
use crate::ast::base::Module;
use crate::parser::Parser;
use anyhow::Result;

pub fn compile(sess: &mut Session, source: &str) -> Result<()> {
    let token = lexer(sess, source);
    let ast = parser(sess, token);
    dbg!(ast);
    Ok(())
}


fn lexer(sess: &mut Session, source: &str) -> Vec<AllyToken>{
    let mut lexer = Lexer::new(source);
    let tokens = lexer.tokenize_all(sess);

    for token in &tokens{
        match token{
            crate::lexer::AllyToken::Identifier(sym) => {
                println!("Identifier: {} (ID: {:?})", sess.lookup(*sym), sym);
            }
            other => println!("Token: {other:?}"),
        }
    }
    
    tokens
}

fn parser(sess: &mut Session, tokens: Vec<AllyToken>) -> Module{
    let mut parser = Parser::new(tokens);
    parser.parse_module()
}
