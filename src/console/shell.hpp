#if !defined(SHELL_H)

#define SHELL_H

#include <string>
#include <vector>
#include <cstdint>

#include "../core/klog.hpp"
#include "../misc/colorizer.hpp"
#include "../misc/tty.hpp"

#include "./commands/echo.hpp"
#include "./commands/help.hpp"
#include "./commands/kconfig.hpp"
#include "./commands/sleep.hpp"

namespace console
{
    class shell
    {
    public:
        void start()
        {
            misc::tty tty;
            src::command::help help;

            tty.clear();

            tty.setShellPrefix("seed@shell" + misc::colorizeFont(">", 180, 180, 40) + " ");

            tty.createSystemCommand("system", &src::command::systemConfig);
            help.commandRegister("SYSTEM", "Show system information.");

            tty.createSystemCommand("sleep", &src::command::systemSleep);
            help.commandRegister("SLEEP", "Pause execution of system.");

            tty.createSystemCommand("echo", &src::command::systemEcho);
            help.commandRegister("ECHO", "Print given text.");

            tty.createSystemCommand("help", [&help](std::vector<std::string> arguments)
            {
                help.systemHelp(arguments);
            });
            help.commandRegister("HELP", "Show list of all system commands.");

            tty.createSystemCommand("clear", [&](std::vector<std::string> arguments)
            {
                tty.clear();
            });
            help.commandRegister("CLEAR", "Clear shell from all recent information.");
            
            tty.createSystemCommand("exit", [&tty](std::vector<std::string> arguments)
            {
                tty.exitSystem();
            });
            help.commandRegister("EXIT", "Shutdown system.");

            core::print(misc::colorizeFont("Seed has sprouted!\n", 180, 180, 40));

            tty.startSystem();
        }
    };
}

#endif