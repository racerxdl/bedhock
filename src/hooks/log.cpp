//
// Created by lucas on 12/12/2021.
//

#include <event/log.h>
#include <fmt/format.h>
#include <fmt/color.h>
#include <hook.h>
#include <log.h>

#include <cstdarg>
#include <iostream>

#define BUFF_SIZE 1024

bool Log::debugEnable = false;

void *Hook::bedLog(LogCategory category, int bitset, LogRule rules, LogAreaID area, unsigned int unk0, char *functionName, int functionLine, char *format, ...) {
    va_list args;
    char buffer[BUFF_SIZE];

    va_start(args, format);
    vsnprintf(buffer, BUFF_SIZE, format, args);
    va_end(args);

    auto data = std::string(buffer);
    auto func = std::string(functionName);
    if (data[data.length()-1] == '\n') {
        data = data.substr(0, data.length()-1);
    }

    WriteOutputEvent(std::make_shared<LogEvent>((int)category, bitset, (int)rules, (int)area, unk0, func, functionLine, data));
    Log::Bedlog("[BDS] {}\n", data);
    return nullptr;
}
