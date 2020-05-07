use std::collections::BTreeMap;
use std::fs::read_to_string;

#[derive(Clone)]
pub struct Node {
    value: &'static str,
    counter: usize,
    tree: BTreeMap<&'static str, Node>,
}

impl Node {
    pub fn new() -> Node {
        Node {
            value: &"",
            counter: 0,
            // tree: BTreeMap::<&'a str, Node<'a>>::new(),
            tree: BTreeMap::new()
        }
    }

    pub fn from(v: &'static str) -> Node {
        Node {
            value: v,
            counter: 0,
            tree: BTreeMap::new()
            // tree: BTreeMap::<&str, Node<'a>>::new(),
        }
    }

    // pub fn get_tree(self) -> auto {
    //     &self.tree.keys()
    // }
    pub fn get(self, val:&str) -> &Node {
        self.tree.get(val).unwrap()
    }

    pub fn inc(&mut self) {
        self.counter += 1
    }

    pub fn update(&mut self, value: &'static str) {
        if self.tree.get(value).is_none() {
            println!("here!")
        }
    }
}