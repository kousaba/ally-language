use crate::session::Session;
use crate::lexer::{Lexer, AllyToken};
use crate::ast::base::Module;
use crate::parser::Parser;
use crate::sema::analyzer::Analyzer;
use crate::mir::builder::MirBuilder;
use crate::mir::syntax::MirBody;
use std::time::Instant;
use anyhow::Result;

pub fn compile(sess: &mut Session, source: &str) -> Result<()> {
    let mut start = Instant::now();
    let token = lexer(sess, source);
    let lexer_duration = start.elapsed();
    start = Instant::now();
    let mut ast = parser(sess, token);
    let parser_duration = start.elapsed();
    dbg!(&ast);
    start = Instant::now();
    semantic_analysis(sess, &mut ast);
    let sema_duration = start.elapsed();
    start = Instant::now();
    dbg!(&ast);
    let mir = mir_build(sess, &mut ast);
    let mir_duration = start.elapsed();
    dbg!(&mir);
    println!("Lexer: {:?}ms", lexer_duration.as_millis());
    println!("Parser: {:?}ms", parser_duration.as_millis());
    println!("SemanticAnalysis: {:?}ms", sema_duration.as_millis());
    println!("MirBuild: {:?}ms", mir_duration.as_millis());
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

fn semantic_analysis(sess: &mut Session, ast: &mut Module){
    let mut analyzer = Analyzer::default();
    analyzer.analyze(ast);
}

fn mir_build(sess: &mut Session, ast: &mut Module) -> Vec<MirBody>{
    let mut mir_bodies = Vec::new();
    let functions = std::mem::take(&mut ast.functions);
    for func in functions{
        let builder = MirBuilder::new();
        let mir_body = builder.build_func(func);
        mir_bodies.push(mir_body);
    }
    mir_bodies
}
