#if !defined(COMMAND_KCONFIG_H)

#define COMMAND_KCONFIG_H

#include <thread>
#include <string>

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
            core::log("OS           : " + misc::colorizeFont(systemName, 180, 180, 40) + "\n");
            core::log("Release      : " + misc::colorizeFont(systemVersion, 180, 180, 40) + "\n");
            core::log("Commands     : " + misc::colorizeFont(std::to_string(tty.commandsCount()), 180, 180, 40) + "\n");
            core::log("Threads      : " + misc::colorizeFont(std::to_string(threads ? threads : 1), 180, 180, 40) + "\n");
            core::log("Uptime       : " + misc::colorizeFont(std::to_string(ktime::now_ms()/1000) + " s", 180, 180, 40) + "\n");
            core::log("Countributors: " + misc::colorizeFont(systemAuthors, 180, 180, 40) + "\n");

            core::log("\n");
            core::log(misc::colorizeBackground("  ", 255, 0, 0) + misc::colorizeBackground("  ", 0, 255, 0) +  misc::colorizeBackground("  ", 0, 0, 255) + "\n");
        }
    }
}

#endif