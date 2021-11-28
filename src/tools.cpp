#include "tools.h"
#include <string>
#include <iostream>
#include <fstream>

std::string currentExecutableName() {
#if defined(PLATFORM_POSIX) || defined(__linux__) //check defines for your setup

    std::string sp;
    std::ifstream("/proc/self/cmdline") >> sp;
    return sp;

#elif defined(_WIN32)

    char buf[MAX_PATH];
    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    return buf;

#else

    static_assert(false, "unrecognized platform");

#endif
}