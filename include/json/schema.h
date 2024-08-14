#pragma once

#include <map>
#include <string>
#include <utility>
#include <variant>
#include <vector>

#include "sanity.h"

namespace json {

using Buffer = std::vector<char>;

struct True
{};

struct False
{};

struct Null
{};

struct Json;
struct Object;
struct Array;
struct String;
struct Number;

using Element = std::variant<False, True, Object, Array, String, Number, Null>;

struct String
{
  Buffer* buffer;
  size_t begin;
  size_t end;

  String(Buffer* buffer, unsigned begin, unsigned end)
    : buffer(buffer)
    , begin(begin)
    , end(end)
  {
  }

  [[nodiscard]] std::string_view data() const;

  [[nodiscard]] bool operator<(const String& rhs) const
  {
    return data() < rhs.data();
  }
};

struct Number : public String
{
  Number(Buffer* buffer, unsigned begin, unsigned end)
    : String(buffer, begin, end)
  {
  }
};

struct Object
{
  std::map<String, Element> members;
};

struct Array
{
  std::vector<Element> elements;
};

struct Json : public Element
{
  std::vector<char> buffer;

  explicit Json()
    : Element()
    , buffer()
  {
  }

  Json& operator=(Element e)
  {
    Element::operator=(std::move(e));
    return *this;
  }
};

std::string_view
String::data() const
{
  return { buffer->data() + begin, end - begin };
}
}

#ifdef JSON_DEFINE_OSTREAM
#include "internal/schema_print.h"
#endif
