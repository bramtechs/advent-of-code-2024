use std::fs::read_to_string;

#[derive(PartialEq, Debug)]
enum Token {
    Mul,
    ParenOpen,
    Comma,
    Number(i32),
    Space,
    ParenClose,
    Other,
    Eof,
}

fn parse_number(view: &[u8]) -> i32 {
    let mut n = 0;
    for c in view {
        if *c == b' ' {
            break;
        }
        n = n * 10 + (*c - b'0') as i32;
    }
    n
}

fn scan_tokens(view: &[u8], tokens: &mut Vec<Token>) {
    let mut token = Token::Other;

    if view.starts_with(b"mul") {
        token = Token::Mul;
    } else {
        let rest = view.trim_start_matches(char::is_numeric);
        if rest.len() < view.len() {
            let diff = &view[0..view.len() - rest.len()];
            token = Token::Number(diff.parse::<i32>().unwrap());
        } else {
            token = match view.first() {
                Some(t) if *t == b'(' => Token::ParenOpen,
                Some(t) if *t == b')' => Token::ParenClose,
                Some(t) if *t == b',' => Token::Comma,
                Some(t) if *t == b' ' => Token::Space,
                Some(_) => Token::Other,
                None => Token::Eof,
            }
        }
    }

    if token != Token::Eof {
        let len = match token {
            Token::Mul => 3,
            Token::Number(n) => n.to_string().len(),
            _ => 1,
        };
        let rest = &view[len..view.len() - 1];
        return scan_tokens(&rest, tokens);
    }

    println!("{:?}", token);
    tokens.push(token);
}

pub fn solve(path: &str) {
    read_to_string(path)
        .expect(format!("Failed to read file {}", path).as_str())
        .lines()
        .map(str::as_bytes)
        .map(|line| {
            let mut v = Vec::new();
            scan_tokens(line, &mut v);
            return v;
        })
        .for_each(|l| println!("{:?}", l));
}
