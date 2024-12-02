// day02.rs

use std::fs::read_to_string;

fn is_report_safe(report: &Vec<i32>) -> bool {
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
}

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

    let safe_part1: Vec<Vec<i32>> = levels.clone().into_iter().filter(is_report_safe).collect();
    let safe_part2: Vec<Vec<i32>> = levels
        .clone()
        .into_iter()
        .filter(|report| {
            if is_report_safe(report) {
                return true;
            }
            for i in 0..report.len() {
                let mut short_report = report.clone();
                short_report.remove(i);
                if is_report_safe(&short_report) {
                    return true;
                }
            }
            return false;
        })
        .collect();

    println!("Day 02 - Part 01 - Safe reports: {}", safe_part1.len());
    println!(
        "Day 02 - Part 02 - Safe reports allowing one mistake: {}",
        safe_part2.len()
    )
}
