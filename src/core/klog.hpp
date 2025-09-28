#if !defined(KLOG_H)

#define KLOG_H

#include <string>
#include <iostream>

#include "../misc/colorizer.hpp"
#include "../time/clock.hpp"

namespace core
{
    enum class Level { INFO, WARN, ERR, DEBUG };

    const std::string levelToString (Level lvl)
    {
        switch (lvl)
        {
            case Level::INFO: return misc::colorizeFont("INFO", 180, 180, 40);
            case Level::WARN: return misc::colorizeFont("WARN", 180, 40, 0);
            case Level::ERR: return misc::colorizeFont("ERR", 180, 40, 40);
            case Level::DEBUG: return misc::colorizeFont("DEBUG", 40, 40, 40);
        }
        return "?";
    }

    void print (std::string message)
    {
        std::cout << message;
    }

    void log (Level lvl, std::string& message)
    {
        std::cout << "[" << levelToString(lvl) << "][" << ktime::systemTime() << "] " << message;
    }

    void info (std::string message) { log(Level::INFO, message); }
    void warn (std::string message) { log(Level::WARN, message); }
    void err (std::string message) { log(Level::ERR, message); }
    void debug (std::string message) { log(Level::DEBUG, message); }
}

#endif