#if !defined(KLOG_H)

#define KLOG_H

#include <string>
#include <iostream>

namespace core
{
    void log(std::string content)
    {
        std::cout << content;
    }
    void input(std::string& inputContent)
    {
        std::getline(std::cin, inputContent);
    }
    void clear()
    {
        system("cls");
    }
}

#endif