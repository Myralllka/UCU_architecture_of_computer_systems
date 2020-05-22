use std::collections::BTreeMap;
// use std::fs::read_to_string;

#[derive(Clone)]
pub struct Node<T> {
    value: T,
    counter: usize,
    tree: BTreeMap<T, Node<T>>,
}

impl<T> Node<T> {

    pub fn new() -> Node<T> {
        Node<T> {

        }
    }

    pub fn from(v: T) -> Node<T> {
        Node {
            value: v,
            counter: 0,
            tree: BTreeMap::<T, Node<T>>
        }
    }

    // pub fn get_tree(self) -> auto {
    //     &self.tree.keys()
    // }

    pub fn get(self, key:T) -> &'static Node<T> {
        self.tree.get(key).unwrap()
    }

    pub fn inc(&mut self) {
        self.counter += 1
    }

    pub fn update(&mut self, value: String) {
        if self.tree.get(&value.clone()).is_none() {
            println!("here!")
        }
    }
}