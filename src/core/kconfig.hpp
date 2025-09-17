#if !defined(KCONFIG_H)

#define KCONFIG_H

#include <thread>
#include <string>
#include "../misc/colorizer.hpp"
//#include "../console/shell.hpp"

namespace src
{
    //auto threads = std::thread::hardware_concurrency();

    const std::string systemVersion = "0.00 - predev";
    const std::string systemName = "SeedOS";
    const std::string systemAuthors = "Daniil Romanov, Maksim Ruletow";

    std::string systemConfig() 
    {
        std::string outputInfo = "OS: " + misc::colorizeFont(systemName, 180, 180, 40) + "\n";
        outputInfo += "Release: " + misc::colorizeFont(systemVersion, 180, 180, 40) + "\n";
        outputInfo += "Countributors: " + misc::colorizeFont(systemAuthors, 180, 180, 40) + "\n\n";
        outputInfo += misc::colorizeBackground("  ", 255, 0, 0) + misc::colorizeBackground("  ", 0, 255, 0) +  misc::colorizeBackground("  ", 0, 0, 255) + "\n";

        //outputInfo += "Uptime: " + std::to_string(hal::now_ms()/1000) + " s\n";
        //outputInfo += "Commands: " + std::to_string(shell::command_count()) + "\n";
        
        //outputInfo += "Threads: " + std::to_string(threads ? threads : 1) + "\n"; // Working without system modules

        return outputInfo;
    }
}

#endif