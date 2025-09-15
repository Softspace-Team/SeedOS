#if !defined(TTY_H)

#define TTY_H

#include <string>
#include <vector>
#include <functional>

#include "../core/kconfig.hpp"
#include "../core/klog.hpp"
#include "../misc/colorizer.hpp"

namespace misc
{
    class tty
    {
    private:
        std::string _prefix = "";
        bool _break = false;
    public:
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

                // DELETE THIS SHIT NOW
                if (userInput == "sys") {
                    core::log(src::systemConfig());
                } else {
                    core::log("I think you write something, but i can't read it for now :(\n");
                }
                // DELETE THIS SHIT NOW

                core::log("\n");
            }
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