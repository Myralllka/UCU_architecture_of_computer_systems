#![allow(unused_variables)]
#![allow(dead_code)]
extern crate ini;

use config_file::Config;
use config_file::read_config_file;

mod config_file;

extern crate tar;
extern crate libarchive;

fn main() {
    let infile: String = std::env::args().nth(1).expect("no filename given");
    let config: Config = read_config_file(infile);
//    println!("{:?}", conf.threads);

}