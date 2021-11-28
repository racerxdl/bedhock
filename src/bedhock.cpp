#include <iostream>
#include <string>

#include "tools.h"
#include "symbolmap.h"
#include "hook.h"

static struct init {
    init() {
        std::string execName = currentExecutableName();
        std::cout << "Executable: " << execName << std::endl;
        SymbolMap symMap(execName);
        Hook::InitSingleton(symMap);
        std::cout << "Bedhock Initialization done" << std::endl;
    }

    ~init() {
        std::cout << "Cleaning up" << std::endl;
    }
} cpp_bedhook_init;
