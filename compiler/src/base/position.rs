use super::span::Span;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Position{
    line: usize,
    column: usize,
}

impl Position{
    pub fn new(line: usize, column: usize) -> Self{
        Self{
            line,
            column,
        }
    }
}
