#if !defined(CONSOLE_H)

#define CONSOLE_H

#include <string>
#include <vector>
#include <cstdint>

#include "../core/clog.hpp"
#include "../misc/tty.hpp"
#include "../misc/system.hpp"

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

            core::log("Seed has sprouted!\n");
            
            tty.startSystem();
        }
    };
}

#endif