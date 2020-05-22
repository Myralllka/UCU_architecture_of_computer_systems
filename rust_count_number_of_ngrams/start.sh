mkdir -p res
cargo build --release
./target/release/count_number_of_ngrams config.conf
