mkdir -p res
cargo build
./target/debug/count_number_of_ngrams config.conf RUST_BACKTRACE=1
