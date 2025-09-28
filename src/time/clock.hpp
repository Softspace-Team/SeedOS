#if !defined(CLOCK_H)

#define CLOCK_H

#include <chrono>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <thread>

namespace ktime
{
    const auto t0 = std::chrono::steady_clock::now();

    std::uint64_t uptime_ms()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - t0 ).count();
    }

    std::string uptime_hms()
    {
        uint64_t ms = uptime_ms();
        uint64_t sec = ms / 1000;
        uint64_t s = sec % 60;
        uint64_t m = (sec % 3600) / 60;
        uint64_t h = sec / 3600;

        char buf[16];

        std::snprintf(buf, sizeof(buf), "%02llu:%02llu:%02llu", (unsigned long long)h, (unsigned long long)m, (unsigned long long)s);

        return std::string(buf);
    }

    std::string systemTime() {
        using namespace std::chrono;
        auto now = system_clock::now();
        std::time_t t = system_clock::to_time_t(now);

        std::tm tm_local{};
#if defined(_WIN32)
        localtime_s(&tm_local, &t);   // Windows
#else
        localtime_r(&t, &tm_local);   // POSIX
#endif

        std::ostringstream oss;
        oss << std::put_time(&tm_local, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    void sleep_ms(std::uint32_t ms)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

#endif