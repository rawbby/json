#pragma once

#include "../json/sanity.h"

#include <istream>

#define JSON2_ADVANCE() (c = stream ? stream.get() : '\0')

void
tokenize(std::istream stream)
{
  char c;
  JSON2_ADVANCE();

  do {
    switch (c) {
      case ' ':
      case '\t':
      case '\r':
      case '\n':
        JSON2_ADVANCE();
        continue;

      case '"': // string
        JSON2_ADVANCE();

        do {
          switch (c) {
          }
        } while (true);

      case '+':
      case '-':
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':

      default:
        ERROR("Invalid Character");
    }
  } while (true);
}
