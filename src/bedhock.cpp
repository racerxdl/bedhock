#include <fmt/format.h>

#include <iostream>
#include <string>
#include <thread>
#include <log.h>

#include "hook.h"
#include "symbolmap.h"
#include "tools.h"

void EventHockThread();

static struct init {
    std::thread *eventHockThread;

    init() {
        std::string execName = currentExecutableName();
        Log::Info("Executable: {}\n", execName);
        SymbolMap symMap;
        symMap.Load();
        Hook::InitSingleton(symMap);

        eventHockThread = new std::thread(EventHockThread);
        Log::Info("Bedhock Initialization done\n");
    }

    ~init() {
        Log::Info("Cleaning up\n");
        Hook::StopAll();
        Log::Info("Waiting eventhock thread to stop\n");
        eventHockThread->join();
        delete eventHockThread;
        Log::Info("Ended cleaning up\n");
    }
} cpp_bedhook_init;
