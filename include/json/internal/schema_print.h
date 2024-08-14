#pragma once

#include "../sanity.h"
#include "../schema.h"

#include <iostream>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

std::ostream&
operator<<(std::ostream& out, const json::Element&);

std::ostream&
operator<<(std::ostream& out, const json::String&);

std::ostream&
operator<<(std::ostream& out, json::False)
{
  return out << "false";
}

std::ostream&
operator<<(std::ostream& out, json::True)
{
  return out << "true";
}

std::ostream&
operator<<(std::ostream& out, const json::Object& object)
{
  constexpr const char* NO_SEPARATOR = "";
  constexpr const char* COMMA_SEPARATOR = ",";

  const char* separator = NO_SEPARATOR;
  out << '{';
  for (const auto& member : object.members) {
    out << separator << member.first << ':' << member.second;
    separator = COMMA_SEPARATOR;
  }
  out << '}';
  return out;
}

std::ostream&
operator<<(std::ostream& out, const json::Array& array)
{
  constexpr const char* NO_SEPARATOR = "";
  constexpr const char* COMMA_SEPARATOR = ",";

  const char* separator = NO_SEPARATOR;
  out << '[';
  for (const auto& element : array.elements) {
    out << separator << element;
    separator = COMMA_SEPARATOR;
  }
  out << ']';
  return out;
}

std::ostream&
operator<<(std::ostream& out, const json::String& string)
{
  return out << '"' << string.data() << '"';
}

std::ostream&
operator<<(std::ostream& out, const json::Number& number)
{
  return out << number.data();
}

std::ostream&
operator<<(std::ostream& out, json::Null)
{
  return out << "null";
}

std::ostream&
operator<<(std::ostream& out, const json::Element& value)
{
  switch (value.index()) {
    case 0:
      return out << std::get<0>(value);
    case 1:
      return out << std::get<1>(value);
    case 2:
      return out << std::get<2>(value);
    case 3:
      return out << std::get<3>(value);
    case 4:
      return out << std::get<4>(value);
    case 5:
      return out << std::get<5>(value);
    case 6:
      return out << std::get<6>(value);
    default:
      ERROR("Invalid variant index");
  }
}

#pragma clang diagnostic pop
