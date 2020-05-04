#[derive(Copy, Clone, Hash)]
pub struct Node {
    value: &'static str,
    counter: usize,
}

impl Node {
    pub fn new() -> Node {
        Node {
            value: &"",
            counter: 0,
        }
    }

    pub fn from(v: &'static str) -> Node {
        Node {
            value: v,
            counter: 0
        }
    }

    pub fn get_counter(self) -> usize {
        self.counter
    }

    pub fn get_value(self) -> &'static str {
        self.value
    }

    pub fn inc(mut self) {
        self.counter += 1
    }
}
