#pragma once

#include "sanity.h"
#include "schema.h"

// hide the massive switch case constructs
#include "internal/tokenizer_macros.h"

#include <fstream>
#include <string_view>

namespace json {

enum TokenType : char
{
  FALSE_CONSTANT = 0,
  TRUE_CONSTANT = 1,
  NULL_CONSTANT = 3,
  STRING = 4,
  NUMBER = 5,
  BEGIN_OBJECT = '{',
  OBJECT_END = '}',
  ARRAY_BEGIN = '[',
  ARRAY_END = ']',
  COMMA = ',',
  COLON = ':'
};

struct Token
{
  TokenType type;
  std::size_t begin;
  std::size_t end;
};

class Tokenizer
{
public:
  explicit Tokenizer(std::istream& file)
    : file(file)
    , current()
  {
    advance();
  }

  inline Token next(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_OBJECT_BEGIN();
        JSON_CASE_PARSE_OBJECT_END();
        JSON_CASE_PARSE_ARRAY_BEGIN();
        JSON_CASE_PARSE_ARRAY_END();
        JSON_CASE_PARSE_COMMA();
        JSON_CASE_PARSE_COLON();
        JSON_CASE_PARSE_STRING();
        JSON_CASE_PARSE_NUMBER();
        JSON_CASE_PARSE_BOOLEAN();
        JSON_CASE_PARSE_NULL();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  inline Token nextValue(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_VALUE();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  inline Token nextColon(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_COLON();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  inline Token nextString(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_STRING();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  inline Token nextStringOrObjectEnd(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_STRING();
        JSON_CASE_PARSE_OBJECT_END();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  inline Token nextCommaOrObjectEnd(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_COMMA();
        JSON_CASE_PARSE_OBJECT_END();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  inline Token nextCommaOrArrayEnd(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_COMMA();
        JSON_CASE_PARSE_ARRAY_END();
        default:
          ERROR("Unexpected character");
      }
    }
  }

  Token nextValueOrArrayEnd(Buffer* buffer)
  {
    std::size_t start = position(buffer);
    while (true) {
      switch (current) {
        JSON_CASE_PARSE_WHITESPACE();
        JSON_CASE_PARSE_VALUE();
        JSON_CASE_PARSE_ARRAY_END();
        default:
          ERROR("Unexpected character");
      }
    }
  }

private:
  std::istream& file;
  char current;

  inline void advance()
  {
    current = file ? static_cast<char>(file.get()) : '\0';
  }

  inline static std::size_t position(Buffer* buffer)
  {
    return buffer->size();
  }
};
}
