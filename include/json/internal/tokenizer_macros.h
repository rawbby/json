#pragma once

#define JSON_CASE_ONE_NINE                                                                                                                                                         \
  case '1':                                                                                                                                                                        \
  case '2':                                                                                                                                                                        \
  case '3':                                                                                                                                                                        \
  case '4':                                                                                                                                                                        \
  case '5':                                                                                                                                                                        \
  case '6':                                                                                                                                                                        \
  case '7':                                                                                                                                                                        \
  case '8':                                                                                                                                                                        \
  case '9'

#define JSON_CASE_DIGIT                                                                                                                                                            \
  case '0':                                                                                                                                                                        \
    JSON_CASE_ONE_NINE

#define JSON_CASE_HEX                                                                                                                                                              \
  JSON_CASE_DIGIT : case 'A':                                                                                                                                                      \
  case 'B':                                                                                                                                                                        \
  case 'C':                                                                                                                                                                        \
  case 'D':                                                                                                                                                                        \
  case 'E':                                                                                                                                                                        \
  case 'F':                                                                                                                                                                        \
  case 'a':                                                                                                                                                                        \
  case 'b':                                                                                                                                                                        \
  case 'c':                                                                                                                                                                        \
  case 'd':                                                                                                                                                                        \
  case 'e':                                                                                                                                                                        \
  case 'f'

#define JSON_CASE_PARSE_WHITESPACE()                                                                                                                                               \
  case ' ':                                                                                                                                                                        \
  case '\t':                                                                                                                                                                       \
  case '\n':                                                                                                                                                                       \
  case '\r':                                                                                                                                                                       \
    advance();                                                                                                                                                                     \
    continue

#define JSON_CASE_PARSE_OBJECT_BEGIN()                                                                                                                                             \
  case '{':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::BEGIN_OBJECT, start, start };                                                                                                                              \
    ((void)0)

#define JSON_CASE_PARSE_OBJECT_END()                                                                                                                                               \
  case '}':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::OBJECT_END, start, start };                                                                                                                                \
    ((void)0)

#define JSON_CASE_PARSE_ARRAY_BEGIN()                                                                                                                                              \
  case '[':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::ARRAY_BEGIN, start, start };                                                                                                                               \
    ((void)0)

#define JSON_CASE_PARSE_ARRAY_END()                                                                                                                                                \
  case ']':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::ARRAY_END, start, start };                                                                                                                                 \
    ((void)0)

#define JSON_CASE_PARSE_COMMA()                                                                                                                                                    \
  case ',':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::COMMA, start, start };                                                                                                                                     \
    ((void)0)

#define JSON_CASE_PARSE_COLON()                                                                                                                                                    \
  case ':':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::COLON, start, start };                                                                                                                                     \
    ((void)0)

#define JSON_PARSE_EXPONENT()                                                                                                                                                      \
  switch (current) {                                                                                                                                                               \
    case '+':                                                                                                                                                                      \
    case '-':                                                                                                                                                                      \
    JSON_CASE_DIGIT:                                                                                                                                                               \
      buffer->push_back(current);                                                                                                                                                  \
      advance();                                                                                                                                                                   \
      while (true) {                                                                                                                                                               \
        switch (current) {                                                                                                                                                         \
        JSON_CASE_DIGIT:                                                                                                                                                           \
          buffer->push_back(current);                                                                                                                                              \
          advance();                                                                                                                                                               \
          continue;                                                                                                                                                                \
          default:                                                                                                                                                                 \
            return { TokenType::NUMBER, start, position(buffer) };                                                                                                                 \
        }                                                                                                                                                                          \
      }                                                                                                                                                                            \
    default:                                                                                                                                                                       \
      ERROR("Invalid Symbol in Exponent!");                                                                                                                                        \
  }                                                                                                                                                                                \
  ((void)0)

#define JSON_PARSE_FRACTION_EXPONENT()                                                                                                                                             \
  while (true) {                                                                                                                                                                   \
    switch (current) {                                                                                                                                                             \
    JSON_CASE_DIGIT:                                                                                                                                                               \
      buffer->push_back(current);                                                                                                                                                  \
      advance();                                                                                                                                                                   \
      continue;                                                                                                                                                                    \
      case 'E':                                                                                                                                                                    \
      case 'e':                                                                                                                                                                    \
        buffer->push_back(current);                                                                                                                                                \
        advance();                                                                                                                                                                 \
        JSON_PARSE_EXPONENT();                                                                                                                                                     \
      default:                                                                                                                                                                     \
        return { TokenType::NUMBER, start, position(buffer) };                                                                                                                     \
    }                                                                                                                                                                              \
  }                                                                                                                                                                                \
  ((void)0)

#define JSON_CASE_PARSE_STRING()                                                                                                                                                   \
  case '"':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    while (true) {                                                                                                                                                                 \
      switch (current) {                                                                                                                                                           \
        case '\\':                                                                                                                                                                 \
          buffer->push_back(current);                                                                                                                                              \
          advance();                                                                                                                                                               \
                                                                                                                                                                                   \
          switch (current) {                                                                                                                                                       \
            case '"':                                                                                                                                                              \
            case '\\':                                                                                                                                                             \
            case '/':                                                                                                                                                              \
            case 'b':                                                                                                                                                              \
            case 'f':                                                                                                                                                              \
            case 'n':                                                                                                                                                              \
            case 'r':                                                                                                                                                              \
            case 't':                                                                                                                                                              \
              buffer->push_back(current);                                                                                                                                          \
              advance();                                                                                                                                                           \
              break;                                                                                                                                                               \
            case 'u':                                                                                                                                                              \
              buffer->push_back(current);                                                                                                                                          \
              advance();                                                                                                                                                           \
                                                                                                                                                                                   \
              for (int i = 0; i < 4; ++i) {                                                                                                                                        \
                switch (current) {                                                                                                                                                 \
                JSON_CASE_HEX:                                                                                                                                                     \
                  buffer->push_back(current);                                                                                                                                      \
                  advance();                                                                                                                                                       \
                  break;                                                                                                                                                           \
                  default:                                                                                                                                                         \
                    ERROR("Invalid escape sequence");                                                                                                                              \
                }                                                                                                                                                                  \
              }                                                                                                                                                                    \
              advance();                                                                                                                                                           \
              break;                                                                                                                                                               \
                                                                                                                                                                                   \
            default:                                                                                                                                                               \
              ERROR("Invalid escape sequence");                                                                                                                                    \
          }                                                                                                                                                                        \
                                                                                                                                                                                   \
          continue;                                                                                                                                                                \
        case ' ':                                                                                                                                                                  \
        case '\t':                                                                                                                                                                 \
        case '\n':                                                                                                                                                                 \
        case '\r':                                                                                                                                                                 \
          buffer->push_back(current);                                                                                                                                              \
          advance();                                                                                                                                                               \
          continue;                                                                                                                                                                \
        case '"':                                                                                                                                                                  \
          advance();                                                                                                                                                               \
          return { TokenType::STRING, start, position(buffer) };                                                                                                                   \
        default:                                                                                                                                                                   \
          buffer->push_back(current);                                                                                                                                              \
          if ((current & 0b1000'0000) == 0b1000'0000) {                                                                                                                            \
            if ((current & 0b1110'0000) == 0b1100'0000) {                                                                                                                          \
              advance();                                                                                                                                                           \
              buffer->push_back(current);                                                                                                                                          \
            } else if ((current & 0b1111'0000) == 0b1110'0000) {                                                                                                                   \
              advance();                                                                                                                                                           \
              buffer->push_back(current);                                                                                                                                          \
              advance();                                                                                                                                                           \
              buffer->push_back(current);                                                                                                                                          \
            } else if ((current & 0b1111'1000) == 0b1111'0000) {                                                                                                                   \
              advance();                                                                                                                                                           \
              buffer->push_back(current);                                                                                                                                          \
              advance();                                                                                                                                                           \
              buffer->push_back(current);                                                                                                                                          \
              advance();                                                                                                                                                           \
              buffer->push_back(current);                                                                                                                                          \
            } else {                                                                                                                                                               \
              ERROR("Invalid first byte in utf8 sequence!");                                                                                                                       \
            }                                                                                                                                                                      \
          }                                                                                                                                                                        \
          advance();                                                                                                                                                               \
      }                                                                                                                                                                            \
    }                                                                                                                                                                              \
    ((void)0)

#define JSON_CASE_PARSE_NUMBER()                                                                                                                                                   \
  case '0':                                                                                                                                                                        \
    buffer->push_back(current);                                                                                                                                                    \
    advance();                                                                                                                                                                     \
    switch (current) {                                                                                                                                                             \
      case '.':                                                                                                                                                                    \
        advance();                                                                                                                                                                 \
        JSON_PARSE_FRACTION_EXPONENT();                                                                                                                                            \
      case 'e':                                                                                                                                                                    \
      case 'E':                                                                                                                                                                    \
        advance();                                                                                                                                                                 \
        JSON_PARSE_EXPONENT();                                                                                                                                                     \
    }                                                                                                                                                                              \
    return { TokenType::NUMBER, start, position(buffer) };                                                                                                                         \
  case '-':                                                                                                                                                                        \
    buffer->push_back(current);                                                                                                                                                    \
    advance();                                                                                                                                                                     \
    switch (current) {                                                                                                                                                             \
      case '0':                                                                                                                                                                    \
        buffer->push_back(current);                                                                                                                                                \
        advance();                                                                                                                                                                 \
        switch (current) {                                                                                                                                                         \
          case '.':                                                                                                                                                                \
            advance();                                                                                                                                                             \
            JSON_PARSE_FRACTION_EXPONENT();                                                                                                                                        \
          case 'e':                                                                                                                                                                \
          case 'E':                                                                                                                                                                \
            advance();                                                                                                                                                             \
            JSON_PARSE_EXPONENT();                                                                                                                                                 \
        }                                                                                                                                                                          \
        return { TokenType::NUMBER, start, position(buffer) };                                                                                                                     \
      JSON_CASE_ONE_NINE:                                                                                                                                                          \
        break;                                                                                                                                                                     \
      default:                                                                                                                                                                     \
        ERROR("Unexpected Symbol in negative number!");                                                                                                                            \
    }                                                                                                                                                                              \
  JSON_CASE_ONE_NINE:                                                                                                                                                              \
    buffer->push_back(current);                                                                                                                                                    \
    advance();                                                                                                                                                                     \
    while (true) {                                                                                                                                                                 \
      switch (current) {                                                                                                                                                           \
      JSON_CASE_DIGIT:                                                                                                                                                             \
        buffer->push_back(current);                                                                                                                                                \
        advance();                                                                                                                                                                 \
        continue;                                                                                                                                                                  \
        case '.':                                                                                                                                                                  \
          advance();                                                                                                                                                               \
          JSON_PARSE_FRACTION_EXPONENT();                                                                                                                                          \
        case 'e':                                                                                                                                                                  \
        case 'E':                                                                                                                                                                  \
          advance();                                                                                                                                                               \
          JSON_PARSE_EXPONENT();                                                                                                                                                   \
        default:                                                                                                                                                                   \
          return { TokenType::NUMBER, start, position(buffer) };                                                                                                                   \
      }                                                                                                                                                                            \
    }                                                                                                                                                                              \
    ((void)0)

#define JSON_CASE_PARSE_BOOLEAN()                                                                                                                                                  \
  case 't':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'r');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'u');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'e');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::TRUE_CONSTANT, start, start };                                                                                                                             \
  case 'f':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'a');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'l');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 's');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'e');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::FALSE_CONSTANT, start, start };                                                                                                                            \
    ((void)0)

#define JSON_CASE_PARSE_NULL()                                                                                                                                                     \
  case 'n':                                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'u');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'l');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    ASSERT(current == 'l');                                                                                                                                                        \
    advance();                                                                                                                                                                     \
    return { TokenType::NULL_CONSTANT, start, start };                                                                                                                             \
    ((void)0)

#define JSON_CASE_PARSE_VALUE()                                                                                                                                                    \
  JSON_CASE_PARSE_OBJECT_BEGIN();                                                                                                                                                  \
  JSON_CASE_PARSE_ARRAY_BEGIN();                                                                                                                                                   \
  JSON_CASE_PARSE_STRING();                                                                                                                                                        \
  JSON_CASE_PARSE_NUMBER();                                                                                                                                                        \
  JSON_CASE_PARSE_BOOLEAN();                                                                                                                                                       \
  JSON_CASE_PARSE_NULL()
