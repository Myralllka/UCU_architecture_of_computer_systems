use std::collections::BTreeMap;
use super::Node;

#[derive(Clone, Hash)]
pub struct MTree {
    value: Node,
    children: BTreeMap<String, MTree>,
}

impl MTree {
    pub fn new() -> MTree {
        MTree {
            value: Node::new(),
            children: BTreeMap::<String, MTree>::new(),
        }
    }

    pub fn from(v: &'static str) -> MTree {
        MTree {
            value: Node::from(v),
            children: BTreeMap::<String, MTree>::new(),
        }
    }

    pub fn get_children(self) -> BTreeMap<String, MTree> {
        self.children
    }

    pub fn inc(self, value: &str) {
        self.children[value].value.inc();
    }

    // pub fn update(self, value:&str) {
        // self.children[value].value.inc();
    // }
}