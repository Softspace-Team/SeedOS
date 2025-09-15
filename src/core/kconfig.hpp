#if !defined(KCONFIG_H)

#define KCONFIG_H

#include <thread>
#include <string>
//#include "../console/shell.hpp"
//#include "../misc/colorizer.hpp"

namespace src
{
    //auto cores = std::thread::hardware_concurrency();

    const std::string systemVersion = "0.00 - predev";
    const std::string systemName = "SeedOS";
    const std::string systemAuthors = "Daniil Romanov, Maksim Ruletow";

    std::string systemConfig() 
    {
        std::string outputInfo = "OS: " + systemName + "\n";
        outputInfo += "Release: " + systemVersion + "\n";
        outputInfo += "Countributors: " + systemAuthors + "\n";
        //outputInfo += "Uptime: " + std::to_string(hal::now_ms()/1000) + " s\n";
        //outputInfo += "Commands: " + std::to_string(shell::command_count()) + "\n";
        
        //outputInfo += "Cores: " + std::to_string(cores ? cores : 1) + "\n";

        return outputInfo;
    }
}

#endif