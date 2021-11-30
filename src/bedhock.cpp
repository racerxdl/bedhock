#include <iostream>
#include <string>
#include <thread>

#include "tools.h"
#include "symbolmap.h"
#include "hook.h"

void EventHockThread();

static struct init {
    std::thread *eventHockThread;

    init() {
        std::string execName = currentExecutableName();
        std::cout << "Executable: " << execName << std::endl;
        SymbolMap symMap(execName);
        Hook::InitSingleton(symMap);

        eventHockThread = new std::thread(EventHockThread);

        std::cout << "Bedhock Initialization done" << std::endl;
    }

    ~init() {
        std::cout << "Cleaning up" << std::endl;
        Hook::StopAll();
        std::cout << "Waiting eventhock thread to stop" << std::endl;
        eventHockThread->join();
        delete eventHockThread;
        std::cout << "Ended cleaning up" << std::endl;
    }
} cpp_bedhook_init;
