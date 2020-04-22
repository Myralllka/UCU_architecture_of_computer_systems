extern crate ini;
use ini::Ini;

pub fn make_config_file() {
    let mut conf = Ini::new();
    conf.with_section(None)
        .set("in_file", "input.txt")
        .set("out_by_a", "out_a.txt")
        .set("out_by_n", "out_n.txt")
        .set("threads", "8");
    conf.write_to_file("config.conf").unwrap();
}