#![allow(unused_variables)]
#![allow(dead_code)]
extern crate ini;

//use ini::Ini;
mod config_file;

use config_file::Config;
use config_file::read_config_file;

fn main() {
    let infile: String = std::env::args().nth(1).expect("no filename given");
    let conf: Config = read_config_file(infile);
    println!("{:?}", conf.threads);
}