#if !defined(TTY_H)

#define TTY_H

#include <vector>
#include <string>
#include <functional>

#include "../core/klog.hpp"
#include "../misc/utilities.hpp"

namespace misc
{   
    struct Command
    {
        std::string command;
        std::function<void(std::vector<std::string>)> executor;
    };

    class tty
    {
    private:
        std::vector<Command> _commands;
        std::string _prefix = "";
        bool _break = false;
    public:
        void createSystemCommand(std::string command, std::function<void(std::vector<std::string>)> function)
        {
            int checkExists = this->checkSystemCommand(command);

            if (checkExists != -1)
            {
                this->_commands.at(checkExists).executor = function;
            }
            else
            {
                this->_commands.push_back({command, function});
            }
        }

        void startSystem() 
        {
            while (true) 
            {
                if (this->_break) {
                    break;
                }

                core::log(this->_prefix);

                std::string userInput;
                core::input(userInput);

                std::vector<std::string> parsedCommand = misc::Split(userInput, " ");
                int checkExists = this->checkSystemCommand(parsedCommand.at(0));

                if (checkExists == -1)
                {
                    core::log("Command not found! Try `help` for more information.\n");
                }
                else
                {
                    Command command = this->_commands.at(checkExists);
                    
                    parsedCommand = misc::Delete<std::string>(parsedCommand, 0);

                    command.executor(parsedCommand);
                }

                core::log("\n");
            }
        }

        int checkSystemCommand(std::string command)
        {
            for (int i = 0; i < this->_commands.size(); i++)
            {
                Command existsCommand = this->_commands.at(i);

                if (existsCommand.command.compare(command) == 0)
                {
                    return i;
                }
            }

            return -1;
        }

        int commandsCount()
        {
            return this->_commands.size();
        }

        void setShellPrefix(std::string prefix)
        {
            this->_prefix = prefix;
        }

        void exitSystem()
        {
            this->_break = true;
        }
    };
}

#endif