// day02.rs

use std::fs::read_to_string;

pub fn solve(path: &str) {
    let levels: Vec<Vec<i32>> = read_to_string(path)
        .expect(format!("Failed to read {}", path).as_str())
        .lines()
        .map(|line| {
            line.trim()
                .split_whitespace()
                .map(|it| {
                    it.parse::<i32>()
                        .expect(format!("Parse error while reading {}", it).as_str())
                })
                .collect::<Vec<i32>>()
        })
        .collect();

    let safe: Vec<Vec<i32>> = levels
        .into_iter()
        .filter(|report| {
            // all increasing or decreasing
            if report.is_sorted() || report.is_sorted_by(|a, b| a >= b) {
                for n in 0..(report.len() - 1) {
                    let diff = report[n].abs_diff(report[n + 1]);
                    if diff < 1 || diff > 3 {
                        return false;
                    }
                }
                return true;
            }
            return false;
        })
        .collect();

    println!("Day 02 - Part 01 - Safe reports: {}", safe.len())
}
