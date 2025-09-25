#if !defined(CLOCK_H)

#define CLOCK_H

#include <chrono>
#include <cstdint>
#include <thread>
#include <string>

namespace ktime
{
    inline const auto t0 = std::chrono::steady_clock::now();

    inline std::uint64_t now_ms() {
        return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - t0 ).count();
    }

    inline void sleep_ms(std::uint32_t ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

#endif