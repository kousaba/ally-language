#[derive(Debug, Clone, Copy)]
pub enum ErrorCode{
    E99999, // TEST ERR
}

impl ErrorCode{
    pub fn message(&self) -> &str{
        match self{
            ErrorCode::E99999 => "TEST ERROR",
        }
    }
}
