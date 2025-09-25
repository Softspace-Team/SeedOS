#if !defined(COMMAND_HELP_H)

#define COMMAND_HELP_H

#include <vector>
#include <string>

#include "../../core/klog.hpp"
#include "../../misc/colorizer.hpp"

namespace src
{
    namespace command
    {
        void systemHelp(std::vector<std::string> arguments)
        {
            if (arguments.size() == 0)
            {
                core::log(misc::colorizeFont("List of all avaible commands (+ description)\n\n", 180, 180, 40));
                core::log(misc::colorizeFont("  help command\n", 180, 180, 40));
                core::log(misc::colorizeFont("  command /?      For more information on a specific command\n\n", 180, 180, 40));
                core::log(misc::colorizeFont("?         List all available commands (without description).\n", 180, 180, 40));
                core::log(misc::colorizeFont("SYSTEM    List all properties of system and computer.\n", 180, 180, 40));
                core::log(misc::colorizeFont("SLEEP     Give system some time to relax.\n", 180, 180, 40));
                core::log(misc::colorizeFont("CLEAR     Clear all commands history in kernel.\n", 180, 180, 40));
                core::log(misc::colorizeFont("EXIT      Shutdown system.", 180, 180, 40));
            }
            else {
                if (arguments.at(0).compare("/?") == 0)
                {
                    core::log(misc::colorizeFont("Provides Help information for SeedOS commands.\n\n", 180, 180, 40));
                    core::log(misc::colorizeFont("HELP [command]\n\n", 180, 180, 40));
                    core::log(misc::colorizeFont("  command - Display help information for this command.", 180, 180, 40));
                }
                
                //something else need here
            }
        }
    }
}

#endif