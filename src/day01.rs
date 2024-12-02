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

    println!("Day 01 - Part 01 - total distance: {}", distance);

    let similarity = left
        .iter()
        .map(|l| {
            let matches_right = right.iter().filter(|r| *r == l).count() as i32;
            l * matches_right
        })
        .sum::<i32>();

    println!("Day 01 - Part 02 - similarity score: {}", similarity);
}
