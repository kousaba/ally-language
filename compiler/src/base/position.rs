#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Position{
    pub line: usize,
    pub column: usize,
}

impl Position{
    pub fn new(line: usize, column: usize) -> Self{
        Self{
            line,
            column,
        }
    }
}
