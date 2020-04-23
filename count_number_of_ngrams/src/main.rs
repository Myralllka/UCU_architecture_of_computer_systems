#![allow(unused_variables)]
#![allow(dead_code)]
extern crate ini;

use config_file::*;
use std::fs::File;
use std::io;
use std::io::Read;
use std::collections::BTreeMap;
use std::iter::FromIterator;

mod config_file;
mod linear_program;
extern crate time;
use time::PreciseTime;
use std::str;

//extern crate tar;
//extern crate libarchive;
//use libarchive3_sys::ffi;

fn main() -> io::Result<()> {
    let start = PreciseTime::now();
    //#################################################
    let infile: String = std::env::args().nth(1).expect("no filename given");
    let config: Config = read_config_file(infile);

    //#################################################
    let mut file = File::open(config.infile).unwrap();
    let mut buf = Vec::new();
    file.read_to_end(&mut buf)?;
    let t = PreciseTime::now();
    println!("Loading: {}", start.to(t));
    //#################################################
    // convert binary to string ^_^
    //#################################################
    let tmp = str::from_utf8(&buf).unwrap();
    //#################################################
    let mut data:Vec<String> = Vec::new();
    data.push(tmp.to_string());
    let  result: BTreeMap<String, usize> = linear_program::count_n_grams(&mut data);
//    for (key, value) in result.iter() {
//        println!("{}: {}", key, value);
//    }

    let mut v = Vec::from_iter(result);
    v.sort_by(|&(_, a), &(_, b)| b.cmp(&a));
    let end = PreciseTime::now();
    println!("General: {}", start.to(end));
//    println!("{:?}", v);
    Ok(())
}