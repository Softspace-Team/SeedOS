#if !defined(COMMAND_KCONFIG_H)

#define COMMAND_KCONFIG_H

#include <string>
#include <thread>

#include "../../core/klog.hpp"
#include "../../misc/colorizer.hpp"
#include "../../misc/tty.hpp"
#include "../../time/clock.hpp"

namespace src
{
    namespace command
    {
        misc::tty tty;

        auto threads = std::thread::hardware_concurrency();

        const std::string systemVersion = "0.00 - predev";
        const std::string systemName = "SeedOS";
        const std::string systemAuthors = "Daniil Romanov, Maksim Ruletow";

        void systemConfig(std::vector<std::string> arguments) 
        {
            core::print("OS           : " + misc::colorizeFont(systemName, 180, 180, 40) + "\n");
            core::print("Release      : " + misc::colorizeFont(systemVersion, 180, 180, 40) + "\n");
            core::print("Commands     : " + misc::colorizeFont(std::to_string(tty.commandsCount()), 180, 180, 40) + "\n");
            core::print("Threads      : " + misc::colorizeFont(std::to_string(threads ? threads : 1), 180, 180, 40) + "\n");
            core::print("Uptime       : " + misc::colorizeFont(ktime::uptime_hms(), 180, 180, 40) + "\n");
            core::print("Countributors: " + misc::colorizeFont(systemAuthors, 180, 180, 40) + "\n");

            core::print("\n");
            core::print(misc::colorizeBackground("  ", 255, 0, 0) + misc::colorizeBackground("  ", 0, 255, 0) +  misc::colorizeBackground("  ", 0, 0, 255) + "\n");
        }
    }
}

#endif