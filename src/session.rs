use inkwell::context::Context;
use lasso::{Rodeo, Spur};
use std::path::PathBuf;

pub type Symbol = Spur;

#[derive(Default, Debug, Clone)]
pub struct SessionConfig {
    pub input_file: PathBuf,
    pub output_file: Option<PathBuf>,
    pub opt_level: u32,
    pub emit_llvm: bool,
}

impl SessionConfig{
    pub fn new(input: PathBuf, output: Option<PathBuf>, opt: u32, llvm: bool) -> Self{
        Self{
            input_file: input,
            output_file: output,
            opt_level: opt,
            emit_llvm: llvm,
        }
    }
}

#[derive(Debug)]
pub struct Session {
    pub config: SessionConfig,
    pub llvm_context: Context,
    pub interner: Rodeo,
}

impl Session{
    pub fn new(config: SessionConfig) -> Self {
        Self{
            config, 
            llvm_context: Context::create(),
            interner: Rodeo::default(),
        }
    }

    pub fn intern(&mut self, s: &str) -> Symbol{
        self.interner.get_or_intern(s)
    }

    pub fn lookup(&self, sym: Symbol) -> &str{
        self.interner.resolve(&sym)
    }
}
