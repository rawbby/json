from os.path import dirname, join, abspath

from json_object import *


def main():
    buffer: [str] = []
    buffer.append('int parse(std::istream&is){' + steps)
    post_(buffer, ws_, value_, ws_, eof_)
    buffer.append(steps_end + '}')
    buffer.append(
        'int main(int argc,char**argv){for(int i=1;i<argc;++i){std::ifstream f{argv[i]};std::cout<<parse(f);}}')

    content = ''.join(buffer)
    if auto_decouple:
        content = decouple(content)
    else:
        content = finalise(content)

    with open(join(abspath(dirname(__file__)), 'src', 'main.cpp'), 'w') as f:
        f.write('#include "./definitions.hpp"\n')
        f.write('#include <istream>\n')
        f.write('#include <cstdio>\n')
        f.write('#include <fstream>\n')
        f.write('#include <iostream>\n')
        f.write('namespace{')
        for group, uid in functions.items():
            f.write(f"int {uid}(std::istream& is);")
        for group, uid in functions.items():
            f.write(f"int {uid}(std::istream& is)")
            f.write('{')
            f.write(group)
            f.write('}')
        f.write('}')
        f.write(content)


if __name__ == '__main__':
    auto_decouple = False
    main()
