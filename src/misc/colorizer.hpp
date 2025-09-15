#if !defined(COLORIZER_H)

#define COLORIZER_H

#include <string>

namespace misc
{
    std::string colorizeFont (const std::string content, const ::size_t r, const ::size_t g, const ::size_t b)
    {
        return "\033[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + content + "\033[0m";
    }

    std::string colorizeBackground (const std::string content, const ::size_t r, const ::size_t g, const ::size_t b)
    {
        return "\033[48;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m" + content + "\033[0m";
    }
}

#endif