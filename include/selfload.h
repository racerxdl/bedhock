#pragma once

#include <unordered_map>

struct SelfSymbols {
    std::unordered_map<std::string, void*> functions;
    std::unordered_map<std::string, void*> objects;
    std::unordered_map<std::string, std::string> mangled;
};

SelfSymbols getSelfSymbols();