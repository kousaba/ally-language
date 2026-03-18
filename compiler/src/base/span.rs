#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Span{
    pub start: usize,
    pub end: usize,
}

impl Span{
    pub fn new(start: usize, end: usize) -> Self{
        Self{
            start, end,
        }
    }
    pub fn merge(&mut self, other: Span){
        if other.start < self.start {
            self.start = other.start;
        }
        if self.end < other.end{
            self.end = other.end;
        }
    }
}
