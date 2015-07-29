#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using u16_t = unsigned short int;

std::string LevelFileFor(u16_t lvl) {
    std::stringstream ss;
    ss << "level" << lvl << ".txt";

    return ss.str();
}

int main() {
    const unsigned int lvl = 1;

    std::string level_file = LevelFileFor(lvl);

    std::ifstream is(level_file);
    if (!is)
        return 1;

    std::string line;
    while (is) {
        std::getline(is, line);
        auto s = line.find("tileset=");
        if (s != std::string::npos) {
            s += 7;
            auto e = line.find(',', s);
            std::cout << s << ':' << e << ':' << line.substr(s, e - 7) << std::endl;
        }

        if (line == "data=") {
            std::cout << "DATA" << std::endl;
            break;
        }
    }
}
