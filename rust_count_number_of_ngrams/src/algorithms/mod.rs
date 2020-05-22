mod linear_ngrams;
mod linear_program;
mod parallel_program;

pub use self::linear_ngrams::linear_n_grams::count_n_grams as count_n_grams;
pub use self::parallel_program::*;