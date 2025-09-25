#if !defined(KCONFIG_H)

#define KCONFIG_H

#include <thread>
#include <string>
#include "../misc/colorizer.hpp"
#include "../time/clock.hpp"
//#include "../console/shell.hpp"

namespace src
{
    auto threads = std::thread::hardware_concurrency();

    const std::string systemVersion = "0.00 - predev";
    const std::string systemName = "SeedOS";
    const std::string systemAuthors = "Daniil Romanov, Maksim Ruletow";

    std::string systemConfig() 
    {
        std::string outputInfo = "OS           : " + misc::colorizeFont(systemName, 180, 180, 40) + "\n";
        outputInfo += "Release      : " + misc::colorizeFont(systemVersion, 180, 180, 40) + "\n";
        outputInfo += "Commands     : " + misc::colorizeFont(std::to_string(4), 180, 180, 40) + "\n"; //shell::command_count()
        outputInfo += "Threads      : " + misc::colorizeFont(std::to_string(threads ? threads : 1), 180, 180, 40) + "\n";
        outputInfo += "Uptime       : " + misc::colorizeFont(std::to_string(ktime::now_ms()/1000) + " s", 180, 180, 40) + "\n";
        outputInfo += "Countributors: " + misc::colorizeFont(systemAuthors, 180, 180, 40) + "\n";

        outputInfo += "\n";
        outputInfo += misc::colorizeBackground("  ", 255, 0, 0) + misc::colorizeBackground("  ", 0, 255, 0) +  misc::colorizeBackground("  ", 0, 0, 255) + "\n";

        return outputInfo;
    }
}

#endif