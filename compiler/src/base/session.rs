use lasso::{Rodeo};
use std::path::PathBuf;
use std::fs;
use crate::base::position::Position;

#[derive(Debug)]
pub struct Session{
    pub rodeo: Rodeo,
    pub file_path: PathBuf,
    pub config: CompilerConfig,
    pub source_info: SourceInfo,
}

#[derive(Debug, Default)]
pub struct CompilerConfig{
    pub opt_level: u8,
    pub is_debug: bool,
}

#[derive(Debug)]
pub struct SourceInfo{
    pub source: String,
    pub line_starts: Vec<usize>,
}

impl Session{
    pub fn new(file_path: PathBuf) -> Self{
        Self{
            rodeo: Rodeo::default(),
            file_path: file_path.clone(),
            config: CompilerConfig::default(),
            source_info: SourceInfo::new(file_path),
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

impl SourceInfo{
    pub fn new(source_path: PathBuf) -> Self{
        let source = fs::read_to_string(&source_path).unwrap();
        println!("Source: {source}");
        let mut line_starts = Vec::new();
        line_starts.push(0);
        for c in source.chars(){
            match c{
                '\n' => line_starts.push(0),
                _ => {
                    if let Some(last) = line_starts.last_mut(){
                        *last += 1;
                    }
                }
            }
        }
        Self{
            source,
            line_starts,
        }
    }
    pub fn offset_to_position(&self, offset: usize) -> Option<Position>{
        if offset > self.source.len() { return None; }
        let line_idx = match self.line_starts.binary_search(&offset){
            Ok(idx) => idx,
            Err(idx) => idx - 1,
        };
        let line_starts_offset = self.line_starts[line_idx];
        Some(Position{
            line: line_idx + 1,
            column: offset - line_starts_offset + 1,
        })
    }
    pub fn position_to_offset(&self, pos: Position) -> Option<usize>{
        let line_idx = pos.line.checked_sub(1)?;
        let start_offset = *self.line_starts.get(line_idx)?;
        let offset = start_offset + (pos.column.checked_sub(1)?);

        if offset <= self.source.len(){
            Some(offset)
        }else{
            None
        }
    }
}
