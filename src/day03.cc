#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

static std::string input =
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))";

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

void parse(const std::string_view &input, std::vector<Token> &tokens) {
  if (input.starts_with("mul")) {
    tokens.emplace_back(Token::Type::MUL, "mul");
    parse(input.substr(3), tokens);
  } else if (input.starts_with("(")) {
    tokens.emplace_back(Token::Type::PAREN_OPEN, "(");
    parse(input.substr(1), tokens);
  } else if (input.starts_with(")")) {
    tokens.emplace_back(Token::Type::PAREN_CLOSE, ")");
    parse(input.substr(1), tokens);
  } else if (input.starts_with(",")) {
    tokens.emplace_back(Token::Type::COMMA, ",");
    parse(input.substr(1), tokens);
  } else if (!input.empty()) {
    if (std::isdigit(*input.begin())) {
      int value{};
      size_t i = 0;
      for (; i < std::min(input.length(), size_t(3)); i++) {
        if (!std::isdigit(input.at(i))) {
          break;
        }
      }

      tokens.emplace_back(Token::Type::VALUE, input.substr(0, i), value);
      parse(input.substr(i), tokens);
    } else {
      tokens.emplace_back(Token::Type::UNKNOWN, *input.begin());
      parse(input.substr(1), tokens);
    }
  }
}

int main(int argc, char **argv) {
  std::vector<Token> tokens;
  parse(input, tokens);

  std::cout << "ok";
}