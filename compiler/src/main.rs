use std::env;
use allyc::base::session::Session;

pub fn main() -> anyhow::Result<()>{
    let args: Vec<String> = env::args().skip(1).collect();
    let sess = Session::from_args(args); 
    let source = sess.file_path.to_string_lossy().into_owned();
    println!("Source: {source}");
    dbg!(sess);
    Ok(())
}
