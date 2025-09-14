#if !defined(SYSTEM_H)

#define SYSTEM_H

#include <string>
//#include "../misc/colorizer"

namespace src
{
    const std::string systemVersion = "0.01";
    const std::string systemName = "SeedOS";
    const std::string systemAuthors = "Daniil Romanov, Maksim Ruletow";

    std::string systemInfo() 
    {
        std::string outputIndo = "OS: " + systemName + "\n";
        outputIndo += "Release: " + systemVersion + "\n";
        outputIndo += "Countributors: " + systemAuthors + "\n";

        return outputIndo;
    }
}

#endif