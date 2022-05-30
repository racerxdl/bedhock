#include <elfio/elfio.hpp>
#include <fmt/format.h>

#include "selfload.h"
#include "symbolmap.h"
#include <log.h>

using namespace ELFIO;

SelfSymbols getSelfSymbols() {
    SelfSymbols self;
    std::string filename = "/proc/self/exe";
    elfio reader;

    Log::Info("Loading symbols from {}\n", bold(filename));
    if (!reader.load(filename)) {
        Log::CriticalError("Error loading file {}\n", bold(filename));
        return self;
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
                if (!demangled.empty()) {  // Cache mangling
                    self.mangled[demangled] = name;
                }
                std::string e;
                switch (type) {
                    case STT_FUNC:
                        self.functions[name] = (void *)value;
                        symbolsLoaded++;
                        if (name.length() > 4) {
                            e = name.substr(name.length() - 4);
                            if (e == "D2Ev" && self.functions.count("D1Ev") == 0) {
                                // Workarround for full destructors
                                // Some classes in bedrock server doesnt have full destructor (D1Ev)
                                // So here we map as a D2Ev (partial destructor)
                                // This might be unsafe, but at least doesnt crash
                                // Since this function checks if there is already a D1Ev,
                                // it is not a problem for classes that does have D1Ev
                                name[name.length() - 3] = '1';
                                self.functions[name] = (void *)value;
                            }
                        }
                        break;
                    case STT_OBJECT:
                        self.objects[name] = (void *)value;
                        symbolsLoaded++;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    return self;
}