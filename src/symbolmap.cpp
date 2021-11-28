#include <string>
#include <elfio/elfio.hpp>
#include <cxxabi.h>

#include "symbolmap.h"

using namespace ELFIO;

SymbolMap::SymbolMap() = default;

SymbolMap::SymbolMap(const SymbolMap& map) {
    this->functions = map.functions;
    this->objects = map.objects;
}

SymbolMap::SymbolMap(const std::string &filename) {
    elfio reader;

    if (!reader.load(filename)) {
        std::cerr << "Error loading file " << filename << std::endl;
        return;
    }
    int symbolsLoaded = 0;
    Elf_Half sec_num = reader.sections.size();
    for (int i = 0; i < sec_num; ++i) {
        section *psec = reader.sections[i];
        // Check section type
        if (psec->get_type() == SHT_SYMTAB) {
            const symbol_section_accessor s(reader, psec);
            for (unsigned int j = 0; j < s.get_symbols_num(); ++j) {
                std::string name;
                Elf64_Addr value;
                Elf_Xword size;
                unsigned char bind;
                unsigned char type;
                Elf_Half section_index;
                unsigned char other;

                // Read symbol properties
                s.get_symbol(j, name, value, size, bind, type, section_index, other);
                auto demangled = SymbolMap::demangle(name);
                if (!demangled.empty()) { // Cache mangling
                    mangled[demangled] = name;
                }

                switch (type) {
                    case STT_FUNC:
                        functions[name] = (void *) value;
                        symbolsLoaded++;
                        break;
                    case STT_OBJECT:
                        objects[name] = (void *) value;
                        symbolsLoaded++;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void *SymbolMap::getFunction(const std::string &name) {
    try {
        return this->functions.at(name);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

void *SymbolMap::getObject(const std::string &name) {
    try {
        return this->objects.at(name);
    } catch (const std::out_of_range &) {
        return nullptr;
    }
}

std::string SymbolMap::demangle(const std::string &name) {
    std::string res;
    int status;
    char *buffer = abi::__cxa_demangle(name.c_str(), nullptr, nullptr, &status);
    if (buffer != nullptr) {
        res = std::string(buffer);
        free(buffer);
    }
    return res;
}

// Hacky way :(
std::string SymbolMap::mangle(const std::string &name) {
    try {
        return this->mangled.at(name);
    } catch (const std::out_of_range &) {
        return {};
    }
}