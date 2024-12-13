#include <fstream>
#include <iostream>
#include <numeric>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

class Token {
public:
  enum class Type : int {
    MUL,
    PAREN_OPEN,
    PAREN_CLOSE,
    VALUE,
    COMMA,
    UNKNOWN
  } type;
  std::string text;
  int value;

  Token(Type type, char text, int value = 0)
      : type(type), text(std::to_string(text)), value(value) {}

  Token(Type type, const std::string_view &text, int value = 0)
      : type(type), text(text), value(value) {}
};

static void parse(const std::string_view &input,
                  std::vector<Token> &outTokens) {
  if (input.starts_with("mul")) {
    outTokens.emplace_back(Token::Type::MUL, "mul");
    parse(input.substr(3), outTokens);
  } else if (input.starts_with("(")) {
    outTokens.emplace_back(Token::Type::PAREN_OPEN, "(");
    parse(input.substr(1), outTokens);
  } else if (input.starts_with(")")) {
    outTokens.emplace_back(Token::Type::PAREN_CLOSE, ")");
    parse(input.substr(1), outTokens);
  } else if (input.starts_with(",")) {
    outTokens.emplace_back(Token::Type::COMMA, ",");
    parse(input.substr(1), outTokens);
  } else if (!input.empty()) {
    if (std::isdigit(*input.begin())) {
      size_t i = 0;
      for (; i < std::min(input.length(), size_t(3)); i++) {
        if (!std::isdigit(input.at(i))) {
          break;
        }
      }

      std::string scan(input.substr(0, i));
      int value = std::stod(scan);
      outTokens.emplace_back(Token::Type::VALUE, input.substr(0, i), value);
      parse(input.substr(i), outTokens);
    } else {
      outTokens.emplace_back(Token::Type::UNKNOWN, *input.begin());
      parse(input.substr(1), outTokens);
    }
  }
}

static void lex(std::span<Token> tokens,
                std::vector<std::pair<int, int>> &outMuls) {
  if (tokens.size() < 5) {
    return;
  }

  if (tokens.begin()->type != Token::Type::MUL) {
    lex(tokens.subspan(1), outMuls);
    return;
  }

  if (tokens[1].type != Token::Type::PAREN_OPEN) {
    lex(tokens.subspan(2), outMuls);
    return;
  }

  if (tokens[2].type != Token::Type::VALUE) {
    lex(tokens.subspan(3), outMuls);
    return;
  }

  int firstValue = tokens[2].value;

  if (tokens[3].type != Token::Type::COMMA) {
    lex(tokens.subspan(4), outMuls);
    return;
  }

  if (tokens[4].type != Token::Type::VALUE) {
    lex(tokens.subspan(5), outMuls);
    return;
  }

  int secondValue = tokens[4].value;

  if (tokens[5].type != Token::Type::PAREN_CLOSE) {
    lex(tokens.subspan(6), outMuls);
    return;
  }

  // we found a valid sequence!
  outMuls.emplace_back(firstValue, secondValue);
  lex(tokens.subspan(6), outMuls);
}

int calculate(const std::vector<std::pair<int, int>> &muls) {
  return std::accumulate(muls.begin(), muls.end(), 0,
                         [](int c, const std::pair<int, int> &value) {
                           return c + (value.first * value.second);
                         });
}

int main(int argc, char **argv) {

  std::fstream stream("assets/day03.txt");
  if (!stream) {
    throw std::runtime_error("Failed to read day 3 input");
  }

  int sum = 0;
  for (std::string line; std::getline(stream, line);) {
    std::vector<Token> tokens{};
    parse(line, tokens);

    std::vector<std::pair<int, int>> muls{};
    lex(tokens, muls);

    sum += calculate(muls);
  }

  std::cout << "Day 03 - Part 01 - Sum of multiplications: " << sum << '\n';
}