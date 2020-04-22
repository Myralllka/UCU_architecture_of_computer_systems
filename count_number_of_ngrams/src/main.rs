extern crate ini;
use ini::Ini;

mod make_config;
fn main() {
    make_config::make_config_file();
//    let conf = Ini::load_from_file("config.ini").unwrap();
//
//    let section = conf.section(Some("main")).unwrap();
//    let threads = section.get("threads").unwrap();
//    let green = section.get("infile").unwrap();
//
//    println!("{:?} {:?}", tommy, green);
//
//     iterating
//    for (sec, prop) in &conf {
//        println!("Section: {:?}", sec);
//        for (key, value) in prop {
//            println!("{:?}:{:?}", key, value);
//        }
//    }
}