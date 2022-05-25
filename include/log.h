#pragma once

#include <fmt/color.h>
#include <fmt/format.h>

#include <string>

class Log {
   public:
    static bool debugEnable;

    template <typename... Args>
    static void Info(const std::string fmtString, const Args&... args) {
        fmt::print(fg(fmt::color::aqua), fmtString, args...);
    }

    template <typename... Args>
    static void Error(const std::string fmtString, const Args&... args) {
        fmt::print(fg(fmt::color::red), fmtString, args...);
    }

    template <typename... Args>
    static void Debug(const std::string fmtString, const Args&... args) {
        if (debugEnable) {
            fmt::print(fg(fmt::color::red), fmtString, args...);
        }
    }

    template <typename... Args>
    static void CriticalError(const std::string fmtString, const Args&... args) {
        fmt::print(fg(fmt::color::white) | bg(fmt::color::red), fmtString, args...);
    }

    template <typename... Args>
    static void Bedlog(const std::string fmtString, const Args&... args) {
        fmt::print(fg(fmt::color::aqua), fmtString, args...);
    }
};

template <typename Arg>
static std::string bold(Arg a) {
    return fmt::format(fmt::emphasis::bold, "{}", a);
}