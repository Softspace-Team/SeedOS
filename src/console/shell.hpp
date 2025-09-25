#if !defined(SHELL_H)

#define SHELL_H

#include <string>
#include <vector>
#include <cstdint>

#include "../core/klog.hpp"
#include "../misc/tty.hpp"
#include "../misc/colorizer.hpp"

#include "./commands/systemconfig.hpp"
#include "./commands/sleep.hpp"
#include "./commands/help.hpp"

namespace console
{
    class shell
    {
    public:
        void start()
        {
            misc::tty tty;

            core::clear();

            tty.setShellPrefix("seed@shell> ");

            core::log(misc::colorizeFont("Seed has sprouted!\n", 180, 180, 40));

            tty.createSystemCommand("help", &src::command::systemHelp);

            tty.createSystemCommand("system", &src::command::systemConfig);

            tty.createSystemCommand("sleep", &src::command::systemSleep);

            tty.createSystemCommand("clear", [&tty](std::vector<std::string> arguments)
            {
                core::clear();
            });
            
            tty.createSystemCommand("exit", [&tty](std::vector<std::string> arguments)
            {
                tty.exitSystem();
            });

            tty.startSystem();
        }
    };
}

#endif