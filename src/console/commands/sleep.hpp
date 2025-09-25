#if !defined(COMMAND_SLEEP_H)

#define COMMAND_SLEEP_H

#include <vector>
#include <string>

#include "../../core/klog.hpp"
#include "../../time/clock.hpp"
#include "../../misc/utilities.hpp"

namespace src
{
    namespace command
    {
        void systemSleep(std::vector<std::string> arguments)
        {
            if (arguments.size() == 0)
            {
                core::log("Arguments is empty! Try `help` for more information.");
            }
            else if (misc::IsNumber(arguments.at(0)) == false)
            {
                core::log("Arguments is not a number! Try `help` for more information.");
            }
            else
            {
                int timeForSleep = std::stoi(arguments.at(0)) * 1000;
                core::log("System is on sleep mode for " + std::to_string(timeForSleep/1000) + " s");
                ktime::sleep_ms(timeForSleep);
            }
        }
    }
}

#endif