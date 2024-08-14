#include "json/parser.h"
#include "json/sanity.h"

#include <fstream>
#include <iostream>

int
main(int argc, char** argv)
{
  for (int argi = 1; argi < argc; ++argi) {
    // std::cout << argv[argi] << ':' << std::endl;
    std::ifstream file(argv[argi]);
    ASSERT(file.is_open());

    json::Parser parser{ file };
    json::Json document = parser.parse();

    // #ifdef JSON_DEFINE_OSTREAM
    //     std::cout << document << '\n';
    // #else
    //     for (const auto c : document.buffer)
    //       std::cout << c;
    //     std::cout << std::endl;
    // #endif
  }
}
