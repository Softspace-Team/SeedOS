#if !defined(CLOG_H)

#define CLOG_H

#include <string>
#include <iostream>

//#include "../misc/colorizer"

namespace core 
{
    void log(std::string content) {
        std::cout << content;
    }
    void input(std::string imputContent) {
        std::getline(std::cin, imputContent);
    }
    void clear() {
        system("cls");
    }
}

#endif