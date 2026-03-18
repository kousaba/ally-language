use lasso::{Rodeo};
use std::path::PathBuf;

#[derive(Debug)]
pub struct Session{
    pub rodeo: Rodeo,
    pub file_path: PathBuf,
    pub config: CompilerConfig,
}

#[derive(Debug, Default)]
pub struct CompilerConfig{
    pub opt_level: u8,
    pub is_debug: bool,
}

impl Session{
    pub fn new(file_path: PathBuf) -> Self{
        Self{
            rodeo: Rodeo::default(),
            file_path,
            config: CompilerConfig::default(),
        }
    }
    pub fn from_args(args: Vec<String>) -> Self{
        if args.is_empty() {
            println!("Please enter input file.");
            panic!("no_input_file");
        }
        let mut sess = Session::new(PathBuf::from(&args[0]));
        for arg in args.iter().skip(1){
            println!("arg: {arg}");
            if arg.starts_with("-O"){
                let mut opt_level = arg.chars().skip(2).collect::<String>().parse::<u8>().unwrap();
                println!("Input OptLevel: {opt_level}");
                if arg.len() >= 5 {
                    println!("OptLevel must be less than 100");
                    opt_level = 99;
                }
                sess.config.opt_level = opt_level;
            }else if arg.starts_with("-debug"){
                sess.config.is_debug = true;
            }
            else{
                println!("Unknown Config");
            }
        }
        sess
    }
}
