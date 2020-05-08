#![allow(unused_variables)]
#![allow(dead_code)]
#![allow(unused_mut)]
extern crate ini;
extern crate time;

use config_file::*;
use std::fs::File;
use std::io;
// use std::io::{Read, Write};
use std::io::Read;
// use std::collections::BTreeMap;
// use std::iter::FromIterator;
use time::PreciseTime;
use std::str;
use crate::my_collections::Node;
use std::collections::BTreeMap;

mod config_file;
mod algorithms;
mod my_collections;

fn main() -> io::Result<()> {
    let start = PreciseTime::now();
    let infile: String = std::env::args().nth(1).expect("no filename given");
    let config: Config = read_config_file(infile);
    let mut file = File::open(&config.infile).unwrap();
    let mut buf = Vec::new();
    file.read_to_end(&mut buf)?;
    let t = PreciseTime::now();
    println!("Loading: {}", start.to(t));
    //#################################################
    // convert binary to string ^_^
    let tmp = str::from_utf8(&buf).unwrap();
    let mut data: Vec<String> = Vec::new();
    data.push(tmp.to_string());
    //#################################################
    // let mut result: BTreeMap<String, usize> = BTreeMap::new();
    // algorithms::count_n_grams(&mut data, &config.n_grams, &mut result);
    //#################################################
    // let mut result: MTree = MTree::new();
    let mut result: BTreeMap<&str, Node<&str>> = BTreeMap::<&str, Node<&str>>::new();
    algorithms::count_n_grams(&mut data, &config.n_grams, &mut result);
    let end = PreciseTime::now();
    // print(&result, &config)?;
    println!("General: {}", start.to(end));
    Ok(())
}

// fn print(tree: &MTree, config: &Config) -> io::Result<()> {
    // let mut file_a = File::create(&config.out_by_a)?;
    // let mut file_n = File::create(&config.out_by_n)?;

    // for (key, value) in map.iter() {
    //     file_a.write_all(format!("{}\t{}\n", key, value).as_bytes())?;
    // }
    // let mut v = Vec::from_iter(map);
    // v.sort_by(|(_, &a), (_, &b)| b.cmp(&a));
    // for &element in v.iter() {
    //     file_n.write_all(format!("{}\t{}\n", element.0, element.1).as_bytes())?;
    // }
    // Ok(())
// }

// fn print(map: &BTreeMap<String, usize>, config: &Config) -> io::Result<()> {
//     let mut file_a = File::create(&config.out_by_a)?;
//     let mut file_n = File::create(&config.out_by_n)?;
//     for (key, value) in map.iter() {
//         let mut st = format!("{}\t{}", key, value).as_bytes();
// file_a.write_all(format!("{}\t{}\n", key, value).as_bytes())?;
// }
// let mut v = Vec::from_iter(map);
// v.sort_by(|(_, &a), (_, &b)| b.cmp(&a));
// for &element in v.iter() {
//     file_n.write_all(format!("{}\t{}\n", element.0, element.1).as_bytes())?;
// }
// Ok(())
// }
