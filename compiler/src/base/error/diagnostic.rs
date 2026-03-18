use super::severity::Severity;
use super::code::ErrorCode;
use crate::base::span::Span;

pub struct Diagnostic{
    pub code: ErrorCode,
    pub severity: Severity,
    pub message: String,
    pub span: Option<Span>,
    pub notes: Vec<String>,
    pub label: Option<String>,
}

pub struct DiagBuilder<'a>{
    diag: Diagnostic,
    // handler: &'a ErrorHandler,
}

impl<'a> DiagBuilder<'a>{
    pub fn report(code: ErrorCode) -> Self{
        Self{
            diag: Diagnostic{
                code,
                severity: Severity::Error,
                message: code.message().to_string(),
                span: None,
                notes: Vec::new(),
                label: None,
            },
        }
    }

    pub fn with_severity(mut self, severity: Severity) -> Self{
        self.diag.severity = severity;
        self
    }

    pub fn with_message(mut self, msg: impl Into<String>) -> Self{
        self.diag.message = msg.into();
        self
    }

    pub fn with_span(mut self, span: Span) -> Self{
        self.diag.span = Some(span);
        self
    }

    pub fn with_label(mut self, label: impl Into<String>) -> Self{
        self.diag.label = Some(label.into());
        self
    }
    
    pub fn with_note(mut self, note: impl Into<String>) -> Self{
        self.diag.notes.push(note.into());
        self
    }

    pub fn emit(self){
        let d = self.diag;
        let prefix = match d.severity{
            Severity::Error => "\x1b[31merror\x1b[0m",
            Severity::Warning =>  "\x1b[33mwarning\x1b[0m",
            _ => "info",
        };
        println!("{}[{:?}]: {}", prefix, d.code, d.message);
        if let Some(span) = d.span {
            println!("  --> at bytes {}..{}", span.start, span.end);
        }

        if let Some(label) = d.label {
            println!("      | labels: {}", label);
        }

        for note in d.notes {
            println!("      = note: {}", note);
        }
    }
}
