#include <fstream>
#include <iostream>
#include <istream>
#include <string_view>
#include <vector>

enum TokenType
{
  RULE,
  ALTERNATIVE_END,
  ITEM,
  TOKEN_BEGIN,
  TOKEN_NAME
};

struct Token
{
  TokenType type;
  unsigned long long begin;
  unsigned long long end;
};

void
tokenize(std::istream& file, std::vector<char>& buffer, std::vector<Token>& result)
{
  char c = file ? static_cast<char>(file.get()) : '\0';
  do {
    switch (c) {
      case 'a':
      case 'b':
      case 'c':
      case 'd':
      case 'e':
      case 'f':
      case 'g':
      case 'h':
      case 'i':
      case 'j':
      case 'k':
      case 'l':
      case 'm':
      case 'n':
      case 'o':
      case 'p':
      case 'q':
      case 'r':
      case 's':
      case 't':
      case 'u':
      case 'v':
      case 'w':
      case 'x':
      case 'y':
      case 'z':
        buffer.push_back(c);
        c = file ? static_cast<char>(file.get()) : '\0';

        do {
          switch (c) {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
              buffer.push_back(c);
              c = file ? static_cast<char>(file.get()) : '\0';
              break;

            case '\n':
              result.push_back({ TokenType::RULE, 0, buffer.size() });

              return;

            default:
              exit(1);
          }
        } while (true);

      default:
        exit(1);
    }
  } while (true);
}

int
main()
{
  std::ifstream file{ "grammar.grammar" };
  if (!file.is_open())
    exit(2);
  std::vector<char> buffer;
  std::vector<Token> result;

  tokenize(file, buffer, result);

  for (const auto& token : result) {
    switch (token.type) {
      case RULE:
        std::cout << std::string_view{ buffer.data() + token.begin, token.end - token.begin } << std::endl;
        break;
      case ALTERNATIVE_END:
        std::cout << std::endl;
        break;
      case ITEM:
        std::cout << ' ' << std::string_view{ buffer.data() + token.begin, token.end - token.begin };
        break;
      case TOKEN_BEGIN:
        std::cout << '(';
        break;
      case TOKEN_NAME:
        std::cout << ':' << std::string_view{ buffer.data() + token.begin, token.end - token.begin } << ')';
        break;
    }
  }
}
