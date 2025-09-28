#if !defined(COMMAND_HELP_H)

#define COMMAND_HELP_H

#include <string>
#include <vector>
#include <cctype>

#include "../../core/klog.hpp"
#include "../../misc/colorizer.hpp"
#include "../../misc/utilities.hpp"

namespace src
{
    namespace command
    {
        struct Command
        {
            std::string name;
            std::string description;
        };
        class help
        {
        private:
            std::vector<Command> _commandRegistry;
        public:
            void commandRegister(const std::string& name, const std::string& description)
            {
                this->_commandRegistry.push_back({name, description});
            }
            void systemHelp(std::vector<std::string>& arguments)
            {
                if (arguments.empty())
                {
                    core::print("List of all available commands:\n");
                    for (auto commandItem : this->_commandRegistry)
                    {
                        core::print(commandItem.name + " - " + commandItem.description + "\n");
                    }
                }
                else
                {
                    std::string target = arguments.at(0);
                    for (auto commandItem : this->_commandRegistry)
                    {
                        if (target == misc::ToLowerString(commandItem.name))
                        {
                            core::print("Provides Help information for SeedOS commands.\n\n");
                            core::print("    " + commandItem.name + " - " + commandItem.description + "\n");
                            return;
                        }
                    }
                    core::warn("No help available for command: " + arguments.at(0) + "\n");
                }
            }
        };
    }
}

#endif