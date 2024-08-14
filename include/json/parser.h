#pragma once

#include "schema.h"
#include "tokenizer.h"
#include <fstream>
#include <vector>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "-Wswitch"
#pragma ide diagnostic ignored "misc-no-recursion"

namespace json {
class Parser
{
public:
  explicit Parser(std::istream& file)
    : tokenizer(file)
    , current()
  {
  }

  Json parse()
  {
    Json document;
    Buffer* buffer = &document.buffer;

    next(buffer);
    switch (current.type) {

      case TokenType::BEGIN_OBJECT:
        document = parseObject(buffer);
        break;

      case TokenType::ARRAY_BEGIN:
        document = parseArray(buffer);
        break;

      case TokenType::STRING:
      case TokenType::NUMBER:
      case TokenType::FALSE_CONSTANT:
      case TokenType::TRUE_CONSTANT:
      case TokenType::NULL_CONSTANT:
        document = parseValue(buffer);
        break;

      default:
        ERROR("Invalid JSON document");
    }

    return document;
  }

private:
  Tokenizer tokenizer;
  Token current;

  inline void next(Buffer* buffer)
  {
    current = tokenizer.next(buffer);
  }
  inline void nextColon(Buffer* buffer)
  {
    current = tokenizer.nextColon(buffer);
  }
  inline void nextString(Buffer* buffer)
  {
    current = tokenizer.nextString(buffer);
  }
  inline void nextStringOrObjectEnd(Buffer* buffer)
  {
    current = tokenizer.nextStringOrObjectEnd(buffer);
  }
  inline void nextValue(Buffer* buffer)
  {
    current = tokenizer.nextValue(buffer);
  }
  inline void nextCommaOrObjectEnd(Buffer* buffer)
  {
    current = tokenizer.nextCommaOrObjectEnd(buffer);
  }
  inline void nextCommaOrArrayEnd(Buffer* buffer)
  {
    current = tokenizer.nextCommaOrArrayEnd(buffer);
  }
  inline void nextValueOrArrayEnd(Buffer* buffer)
  {
    current = tokenizer.nextValueOrArrayEnd(buffer);
  }

  Object parseObject(Buffer* buffer)
  {
    Object object;

    nextStringOrObjectEnd(buffer);
    switch (current.type) {
      case TokenType::OBJECT_END:
        return object;
      default: // string
        const auto key = String(buffer, current.begin, current.end);
        nextColon(buffer);
        nextValue(buffer);
        object.members[key] = parseValue(buffer);
        nextCommaOrObjectEnd(buffer);
    }

    while (current.type != TokenType::OBJECT_END) {
      nextString(buffer);
      const auto key = String(buffer, current.begin, current.end);
      nextColon(buffer);
      nextValue(buffer);
      object.members[key] = parseValue(buffer);
      nextCommaOrObjectEnd(buffer);
    }

    return object;
  }

  Array parseArray(Buffer* buffer)
  {
    Array array;

    nextValueOrArrayEnd(buffer);
    switch (current.type) {
      case TokenType::ARRAY_END:
        return array;
      default: // value
        array.elements.push_back(parseValue(buffer));
        nextCommaOrArrayEnd(buffer);
    }

    while (current.type != TokenType::ARRAY_END) {
      nextValue(buffer);
      array.elements.push_back(parseValue(buffer));
      nextCommaOrArrayEnd(buffer);
    }

    return array;
  }

  Element parseValue(Buffer* buffer)
  {
    Element schema;

    switch (current.type) {
      case TokenType::STRING:
        schema = String(buffer, current.begin, current.end);
        break;
      case TokenType::NUMBER:
        schema = Number(buffer, current.begin, current.end);
        break;
      case TokenType::TRUE_CONSTANT:
        schema = True();
        break;
      case TokenType::FALSE_CONSTANT:
        schema = False();
        break;
      case TokenType::NULL_CONSTANT:
        schema = Null();
        break;
      case TokenType::BEGIN_OBJECT:
        schema = parseObject(buffer);
        break;
      case TokenType::ARRAY_BEGIN:
        schema = parseArray(buffer);
        break;
      default:
        ERROR("Unexpected token");
    }
    return schema;
  }
};
}

#pragma clang diagnostic pop
