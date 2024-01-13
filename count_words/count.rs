use std::io::{BufRead, BufReader};
use std::fs::File;
use std::collections::HashMap;


fn main() {
    // https://rust-cli.github.io/book/tutorial/cli-args.html
    let filename = std::env::args().nth(1).expect("no filename given.");

    let mut words: HashMap<String, u32> = HashMap::new();

    // https://stackoverflow.com/a/39434382
    let reader = BufReader::new(File::open(filename).expect("Cannot open file.txt"));
    for line in reader.lines() {
        for word in line.unwrap().split_whitespace() {
            // println!("word '{}'", word);
            // https://users.rust-lang.org/t/efficient-string-hashmaps-for-a-frequency-count/7752/2
            *words.entry(word.to_string()).or_insert(0) += 1;
        }
    }

    // println!("{:?}", words);
    // println!("{}", filename);

    let mut count_vec: Vec<_> = words.iter().collect();
    // https://stackoverflow.com/questions/34555837/sort-hashmap-data-by-value
    count_vec.sort_by(|a, b| a.1.cmp(b.1));
    // println!("{:?}", count_vec);
    count_vec.iter().for_each(|it| {
        println!("{}: {}", it.0, it.1)
    })

}

// ./count kjvbible.txt  0.45s user 0.02s system 99% cpu 0.467 total