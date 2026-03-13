use ally_lang::session::{Session, SessionConfig};
use std::env;
use std::path::PathBuf;
use clap::Parser;

pub fn main() -> anyhow::Result<()>{
    println!("Compiling...");
    let args: Vec<String> = env::args().collect();
    let config = build_config(&args);
    let mut session = Session::new(config);
    dbg!(&session);
    let source = r#"
    fn main(){
        let a = 0;
        if a > 10{
            ret = 5;
        } else {
            ret = 10;
        }
    }
    "#;
    ally_lang::driver::compile(&mut session, source)?;
    Ok(())
}

#[derive(Parser, Debug)]
#[command(name = "ally-lang", version)]
struct Cli{
    input: PathBuf,
    #[arg(short, long)]
    output: Option<PathBuf>,
    #[arg(short = 'O', default_value = "999")]
    opt_level: u32,
    #[arg(long)]
    emit_llvm: bool,
}

fn build_config(args: &[String]) -> SessionConfig{
    let cli = Cli::parse_from(args);
    SessionConfig{
        input_file: cli.input,
        output_file: cli.output,
        opt_level: cli.opt_level,
        emit_llvm: cli.emit_llvm,
    }
}
