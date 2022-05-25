#pragma once

#include <map>
#include <unordered_map>

class SymbolMap {
   private:
    using symbolMap = std::unordered_map<std::string, void *>;
    symbolMap functions;
    symbolMap objects;
    std::map<std::string, std::string> mangled;

   public:
    SymbolMap();
    SymbolMap(const SymbolMap &map);
    explicit SymbolMap(const std::string &filename);
    void *getFunction(const std::string &name);
    void *getObject(const std::string &name);
    void Load();
    void ProcessCB(struct dl_phdr_info *info, size_t size);

    static std::string demangle(const std::string &name);
    std::string mangle(const std::string &name);
};