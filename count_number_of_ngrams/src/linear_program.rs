use std::collections::BTreeMap;

extern crate unicode_normalization;
extern crate time;
use time::PreciseTime;
use unicode_normalization::UnicodeNormalization;

pub fn count_n_grams(buffer: &mut Vec<String>, n_grams :&usize, result: &mut BTreeMap<String, usize>) {
    // # Input
    // Vector of Strings from each file.
    // # Output
    // BTreeMap, which is easy to print in sorted order
    let start = PreciseTime::now();
    for entry in buffer.iter_mut() {
        let entry = entry.replace(&['(', ')', ',', '.', ';', ':'][..], "").nfc().collect::<String>().to_lowercase();
        for word in entry.split(is_whitespace).filter(is_not_empty) {
            // let tmp = word.replace(&['(', ')', ',', '.', ';', ':'][..], "").nfc().collect::<String>();
            if let Some(count) = result.get_mut(word) {
                *count += 1;
                continue;
            }
            result.insert(word.to_string(), 1);
        }
    }
    let end = PreciseTime::now();
    println!("Analyzing: {}", start.to(end));
}

#[inline]
fn is_not_empty(s: &&str) -> bool {
    !s.is_empty()
}

#[inline]
fn is_whitespace(c: char) -> bool {
    c == ' ' || c == '\t' || c == '\n'
}