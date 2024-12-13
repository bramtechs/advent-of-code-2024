#include <fstream>
#include <iostream>
#include <numeric>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

/**
 * Not a good solution as I had to increase the stack size for the full input...
 */

using Multiplication = std::tuple<int, int, bool>;
using MultiplicationVec = std::vector<Multiplication>;

class Token {
public:
  enum class Type : int {
    MUL,
    PAREN_OPEN,
    PAREN_CLOSE,
    VALUE,
    COMMA,
    UNKNOWN,
    DO,
    DONT
  } type;
  std::string text;
  int value;

  Token(Type type, char text, int value = 0)
      : type(type), text(std::string("") + text), value(value) {}

  Token(Type type, const std::string_view &text, int value = 0)
      : type(type), text(text), value(value) {}
};

static void parse(const std::string_view &input,
                  std::vector<Token> &outTokens) {
  if (input.starts_with("mul")) {
    outTokens.emplace_back(Token::Type::MUL, "mul");
    parse(input.substr(3), outTokens);
  } else if (input.starts_with("don't")) {
    outTokens.emplace_back(Token::Type::DONT, "don't");
    parse(input.substr(5), outTokens);
  } else if (input.starts_with("do")) {
    outTokens.emplace_back(Token::Type::DO, "do");
    parse(input.substr(2), outTokens);
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

static void lex(std::span<Token> tokens, MultiplicationVec &outMuls,
                bool active = true) {
  if (tokens.size() < 5) {
    return;
  }

  // check for do or dont
  if (tokens.begin()->type == Token::Type::DONT &&
      tokens[1].type == Token::Type::PAREN_OPEN &&
      tokens[2].type == Token::Type::PAREN_CLOSE) {
    active = false;
  } else if (tokens.begin()->type == Token::Type::DO &&
             tokens[1].type == Token::Type::PAREN_OPEN &&
             tokens[2].type == Token::Type::PAREN_CLOSE) {
    active = true;
  }

  // check for mul

  if (tokens.begin()->type != Token::Type::MUL) {
    lex(tokens.subspan(1), outMuls, active);
    return;
  }

  if (tokens[1].type != Token::Type::PAREN_OPEN) {
    lex(tokens.subspan(2), outMuls, active);
    return;
  }

  if (tokens[2].type != Token::Type::VALUE) {
    lex(tokens.subspan(3), outMuls, active);
    return;
  }

  int firstValue = tokens[2].value;

  if (tokens[3].type != Token::Type::COMMA) {
    lex(tokens.subspan(4), outMuls, active);
    return;
  }

  if (tokens[4].type != Token::Type::VALUE) {
    lex(tokens.subspan(5), outMuls, active);
    return;
  }

  int secondValue = tokens[4].value;

  if (tokens[5].type != Token::Type::PAREN_CLOSE) {
    lex(tokens.subspan(6), outMuls, active);
    return;
  }

  // we found a valid sequence!
  outMuls.emplace_back(firstValue, secondValue, active);
  lex(tokens.subspan(6), outMuls, active);
}

static int calculate(MultiplicationVec &muls, bool part1) {
  return std::accumulate(
      muls.begin(), muls.end(), 0, [part1](int c, const Multiplication &value) {
        if (std::get<2>(value) || part1) {
          return c + (std::get<0>(value) * std::get<1>(value));
        }
        return c;
      });
}

int main(int argc, char **argv) {

  std::fstream stream("assets/day03.txt");
  if (!stream) {
    throw std::runtime_error("Failed to read day 3 input");
  }

  std::stringstream content{};
  content << stream.rdbuf();

  std::vector<Token> tokens{};
  parse(content.str(), tokens);

  MultiplicationVec muls{};
  lex(tokens, muls);

  int sumPart1 = calculate(muls, true);
  int sumPart2 = calculate(muls, false);

  std::cout << "Day 03 - Part 01 - Sum of multiplications: " << sumPart1
            << '\n';
  std::cout << "Day 03 - Part 02 - Sum of conditional multiplications: "
            << sumPart2 << '\n';
}