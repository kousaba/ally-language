use std::collections::HashMap;
use crate::sema::symbol::Symbol;
use crate::ast::base::Type;

pub struct VariableInfo{
    pub ty: Type,
    pub is_mutable: bool,
}

impl VariableInfo{
    pub fn new(typ: Type, mutable: bool) -> Self{
        Self{ty: typ, is_mutable: mutable}
    }
}

pub struct ScopeStack{
    stack: Vec<HashMap<Symbol, VariableInfo>>,
}

impl ScopeStack{
    pub fn new() -> Self{
        Self { stack: vec![HashMap::new()] }
    }
    pub fn push(&mut self) { self.stack.push(HashMap::new()); }
    pub fn pop(&mut self) { self.stack.pop(); }
    
    pub fn define(&mut self, name: Symbol, info: VariableInfo){
        self.stack.last_mut().unwrap().insert(name, info);
    }

    pub fn lookup(&self, name: Symbol) -> Option<&VariableInfo> {
        // 関数内の二重定義はだめだが、一応逆順
        for scope in self.stack.iter().rev(){
            if let Some(info) = scope.get(&name){
                return Some(info);
            }
        }
        None
    }
}
