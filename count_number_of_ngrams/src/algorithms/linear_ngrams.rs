pub mod linear_n_grams {
    use time::PreciseTime;
    use std::collections::BTreeMap;
    use unicode_normalization::UnicodeNormalization;

    use super::super::super::my_collections::Node;

    extern crate unicode_normalization;
    extern crate time;

    // pub fn count_n_grams(buffer: &mut Vec<String>, n_garms: &usize, mut result: &mut MTree) {
    pub fn count_n_grams(buffer: &mut Vec<String>, n_garms: &usize, mut result: &mut BTreeMap<&str, Node<&str>>) {
        let start = PreciseTime::now();
        // let mut head: &MTree = result;
        result.insert(&"", Node::from(" "));
        let mut head = result.get("").unwrap();
        for entry in buffer.iter_mut() {
            for word in entry.split(is_whitespace).filter(is_not_empty) {
                let mut tmp = word.replace(&['(', ')', ',', ';', ':'][..], "").nfc().collect::<String>().to_lowercase();
                head.update(tmp.clone());
                // result = result.child(tmp.clone().as_str());
                // if is_dot(tmp.chars().last().unwrap()) {} else {}
            }
        }
        let end = PreciseTime::now();
        println!("Analyzing: {}", start.to(end));
    }

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
    fn is_dot(c: char) -> bool {
        c == '.'
    }

    #[inline]
    fn is_whitespace(c: char) -> bool {
        c == ' ' || c == '\t' || c == '\n'
    }
}