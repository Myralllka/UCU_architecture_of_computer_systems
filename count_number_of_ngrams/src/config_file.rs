#![allow(unused_variables)]
#![allow(dead_code)]
extern crate ini;

use ini::Ini;

pub struct Config {
    pub infile: String,
    pub out_by_a: String,
    pub out_by_n: String,
    pub threads: i32,
}

pub fn make_config_file() {
    let none: Option<String> = None;
    let mut conf = Ini::new();
    conf.with_section(none)
        .set("in_file", "input.txt")
        .set("out_by_a", "out_a.txt")
        .set("out_by_n", "out_n.txt")
        .set("threads", "8");
    conf.write_to_file("config.conf").unwrap();
}

pub fn read_config_file(filename: String) -> Config {
    let conf = Ini::load_from_file(filename).unwrap();
    let none: Option<String> = None;
    let section = conf.section(none).unwrap();
    let threads = section.get("threads").unwrap();
    let result: Config = build_conf(section.get("in_file").unwrap().to_string(),
                                    section.get("out_by_a").unwrap().to_string(),
                                    section.get("out_by_n").unwrap().to_string(),
                                    threads.trim().parse().expect("Expect threads as number"));
    result
}

fn build_conf(infile: String, out_by_a: String, out_by_n: String, threads: i32) -> Config {
    Config { infile, out_by_a, out_by_n, threads }
}