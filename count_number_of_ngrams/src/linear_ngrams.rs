use std::collections::BTreeMap;

extern crate unicode_normalization;
extern crate time;

use time::PreciseTime;
use unicode_normalization::UnicodeNormalization;

struct Node {
    value:char,
    counter:usize,
    children:Vec<Node>
}

impl Node {
    pub fn get_counter() -> usize {
        counter
    }

    pub fn get_children() -> usize {
        children
    }

    pub fn get_value() -> char {
        value
    }
}

struct MTree {}

pub fn linear_n_grams(buffer: &mut Vec<String>, n_grams: &usize, result: &mut BTreeMap<String, usize>) {}


pub fn count_2_grams(buffer: &mut Vec<String>, n_grams: &usize, result: &mut BTreeMap<String, usize>) {
    // # Input
    // Vector of Strings from each file.
    // # Output
    // BTreeMap, which is easy to print in sorted order
    let mut prev_word = String::new();
    let empty = String::from("").trim();
    let mut current_word: String;
    let start = PreciseTime::now();
    for entry in buffer.iter_mut() {
        let entry = entry.replace(&['(', ')', ',', '.', ';', ':'][..], "").nfc().collect::<String>().to_lowercase();
        for word in entry.split(is_whitespace).filter(is_not_empty) {
            if prev_word.is_empty() {
                current_word = (&word).to_string();
            } else {
                current_word = format!("{} {}", prev_word, word)
            }
            // let tmp = word.replace(&['(', ')', ',', '.', ';', ':'][..], "").nfc().collect::<String>();
            if let Some(count) = result.get_mut(current_word.trim()) {
                *count += 1;
                continue;
            }
            result.insert(current_word.to_string(), 1);
            prev_word = (&word).to_string();
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

struct Value {
    word: String,
    count: usize,
}

impl Value {
    pub fn from(word: String) -> Value {
        Value(word, 0)
    }

    pub fn get_word() -> String {
        self::word
    }

    pub fn get_count() -> usize {
        self::count
    }

    pub fn inc() {
        self::count += 1;
    }
}