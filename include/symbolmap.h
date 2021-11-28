#pragma once

#include <map>

class SymbolMap {
    std::map<std::string, void *> functions;
    std::map<std::string, void *> objects;
    std::map<std::string, std::string> mangled;

public:
    SymbolMap();
    SymbolMap(const SymbolMap& map);
    explicit SymbolMap(const std::string& filename);
    void *getFunction(const std::string& name);
    void *getObject(const std::string& name);

    static std::string demangle(const std::string &name);
    std::string mangle(const std::string &name);
};