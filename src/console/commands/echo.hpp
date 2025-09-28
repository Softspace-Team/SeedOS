#if !defined(COMMAND_ECHO_H)

#define COMMAND_ECHO_H

#include <string>
#include <vector>

#include "../../core/klog.hpp"
#include "../../misc/utilities.hpp"

namespace src
{
    namespace command
    {
        void systemEcho(std::vector<std::string> arguments)
        {
            core::print(misc::JoinStrings(arguments) + "\n");
        }
    }
}

#endif