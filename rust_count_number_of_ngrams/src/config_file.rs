extern crate ini;

use ini::Ini;
use std::fs;

pub struct Config {
    pub infile: String,
    pub out_by_a: String,
    pub out_by_n: String,
    pub threads: usize,
    pub n_grams: usize,
}

pub fn make_config_file() {
    let none: Option<String> = None;
    let mut conf = Ini::new();
    conf.with_section(none)
        .set("in_file", "input.txt")
        .set("out_by_a", "out_a.txt")
        .set("out_by_n", "out_n.txt")
        .set("threads", "8")
        .set("n_grams", "1");
    conf.write_to_file("config.conf").unwrap();
}

pub fn read_config_file(filename: String) -> Config {
    let conf = Ini::load_from_file(filename).unwrap();
    let none: Option<String> = None;
    let section = conf.section(none).unwrap();
    let threads = section.get("threads").unwrap();
    let n_grams = section.get("n_grams").unwrap();
    let result: Config = build_conf(section.get("in_file").unwrap().to_string(),
                                    section.get("out_by_a").unwrap().to_string(),
                                    section.get("out_by_n").unwrap().to_string(),
                                    threads.trim().parse().expect("Expect threads as a number"),
                                    n_grams.trim().parse().expect("Expect n_grams as a number"));
    result
}

fn build_conf(infile: String, out_by_a: String, out_by_n: String, threads: usize, n_grams: usize) -> Config {
    match path_exists(&infile) {
        true => println!("input file exists, continuing..."),
        false => { panic!("input file does not exists!") }
    };
    match &threads {
        0..=8 => println!("Correct number of threads, continuing..."),
        x if x > &8 => println!("WARNING! too big number of threads, continuing..."),
        _ => { panic!("incorrect number of threads!") }
    };
    match &n_grams {
        x if x > &0 => println!("Correct number of n_grams, continuing..."),
        _ => { panic!("incorrect number of n_grams!") }
    };
    Config { infile, out_by_a, out_by_n, threads, n_grams: n_grams }
}

fn path_exists(path: &str) -> bool {
    fs::metadata(path).is_ok()
}
