//
// Created by lucas on 12/12/2021.
//

#include <hook.h>
#include <cstdarg>
#include <iostream>
#include <event/log.h>

#define BUFF_SIZE 1024

void *Hook::bedLog(LogCategory category, int bitset,LogRule rules,LogAreaID area, unsigned int unk0, char *functionName,int functionLine, char *format,...) {
    va_list args;
    char buffer[BUFF_SIZE];

    va_start(args, format);
    vsnprintf(buffer, BUFF_SIZE, format, args);
    va_end(args);

    auto data = std::string(buffer);
    auto func = std::string(functionName);

    WriteOutputEvent(std::make_shared<LogEvent>((int)category, bitset, (int)rules, (int)area, unk0, func, functionLine, data));
    std::cout << "(SERVER)[" << func << "+" << functionLine << "] " << data << std::endl;

    return nullptr;
}
