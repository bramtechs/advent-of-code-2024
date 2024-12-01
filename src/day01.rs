// day01.rs

use std::fs::read_to_string;

pub fn solve(path: &str) {
    let mut left: Vec<i32> = Vec::new();
    let mut right: Vec<i32> = Vec::new();

    read_to_string(path)
        .expect("Failed to read file")
        .lines()
        .for_each(|line| {
            let segs = line
                .trim()
                .split_whitespace()
                .map(|x| x.parse().expect(format!("Failed to parse {}", x).as_str()))
                .collect::<Vec<i32>>();
            left.push(segs[0]);
            right.push(segs[1]);
        });

    left.sort();
    right.sort();

    let distance = left
        .iter()
        .zip(right.iter())
        .map(|(l, r)| i32::abs(r - l))
        .sum::<i32>();

    println!("Part 1 total distance: {}", distance);
}
