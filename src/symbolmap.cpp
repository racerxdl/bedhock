#include "symbolmap.h"

#include <cxxabi.h>
#include <dlfcn.h>
#include <elf.h>
#include <link.h>
#include <log.h>
#include <sys/auxv.h>
#include <sys/mman.h>

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "selfload.h"

// Tools for loading elf information

struct strTab {
    char *tab;
    ElfW(Xword) size;
};

struct jmpRelTab {
    ElfW(Rela) * tab;
    ElfW(Xword) size;
};

struct symTab {
    ElfW(Sym) * tab;
    ElfW(Xword) entsz;
    uint64_t entries;
};

std::string getLibraryName(void *addr) {
    Dl_info info;
    dladdr(addr, &info);
    return std::string(info.dli_fname);
}

std::string getCurrentLibraryName() {
    return getLibraryName((void *)&getCurrentLibraryName);
}

int callback(struct dl_phdr_info *info, size_t size, void *data) {
    auto sm = (SymbolMap *)data;
    sm->ProcessCB(info, size);
    return 0;
}

const ElfW(Phdr) * getDynamicPHDR(const ElfW(Phdr) * phdr, uint16_t phnum, uint16_t phentsize) {
    int i;

    for (i = 0; i < phnum; i++) {
        if (phdr->p_type == PT_DYNAMIC) {
            return phdr;
        }
        phdr = (ElfW(Phdr) *)((char *)phdr + phentsize);
    }

    return NULL;
}

const ElfW(Dyn) * getDynEntry(ElfW(Addr) base, const ElfW(Phdr) * pdyn, uint32_t type) {
    ElfW(Dyn) * dyn;

    for (dyn = (ElfW(Dyn) *)(base + pdyn->p_vaddr); dyn->d_tag; dyn++) {
        if (dyn->d_tag == type) {
            return dyn;
        }
    }

    return NULL;
}

struct jmpRelTab getJmpRel(ElfW(Addr) base, const ElfW(Phdr) * pdyn) {
    struct jmpRelTab table;
    const ElfW(Dyn) * dyn;

    dyn = getDynEntry(base, pdyn, DT_JMPREL);
    table.tab = (dyn == NULL) ? NULL : (ElfW(Rela) *)dyn->d_un.d_ptr;

    dyn = getDynEntry(base, pdyn, DT_PLTRELSZ);
    table.size = (dyn == NULL) ? 0 : dyn->d_un.d_val;
    return table;
}

int computeNumSymbols(ElfW(Addr) base, const ElfW(Phdr) * pdyn, symTab tab) {
    struct gnu_hash_table {
        uint32_t nbuckets;
        uint32_t symoffset;
        uint32_t bloom_size;
        uint32_t bloom_shift;
    };
    struct hash_table {
        uint32_t nbucket;
        uint32_t nchain;
    };

    const ElfW(Dyn) *dyn = getDynEntry(base, pdyn, DT_HASH);
    if (dyn != nullptr) {
        return 0;  // TODO
    } else {
        dyn = getDynEntry(base, pdyn, DT_GNU_HASH);
        auto hashTable = (gnu_hash_table *)dyn->d_un.d_ptr;
        auto bloomPtr = ((char *)hashTable) + sizeof(gnu_hash_table);
        auto bucketPtr = bloomPtr + sizeof(uint64_t) * hashTable->bloom_size;
        auto chainPtr = bucketPtr + sizeof(uint32_t) * hashTable->nbuckets;

        auto bucket = (uint32_t *)bucketPtr;
        auto chain = (uint32_t *)chainPtr;
        auto symbols = 0;

        std::vector<uint32_t> buckets(hashTable->nbuckets);
        memcpy(buckets.data(), bucket, sizeof(uint32_t) * buckets.size());

        uint32_t last_symbol = 0;
        for (uint32_t i = 0; i < hashTable->nbuckets; ++i) {
            last_symbol = std::max(buckets[i], last_symbol);
        }

        if (last_symbol < hashTable->symoffset) {
            return hashTable->symoffset;
        }
        while (!(chain[last_symbol] & 1))
            last_symbol++;
        symbols = last_symbol;
        return symbols;
    }
}

symTab getSymTab(ElfW(Addr) base, const ElfW(Phdr) * pdyn) {
    symTab table;
    const ElfW(Dyn) * dyn;

    dyn = getDynEntry(base, pdyn, DT_SYMTAB);
    table.tab = (dyn == NULL) ? NULL : (ElfW(Sym) *)dyn->d_un.d_ptr;
    dyn = getDynEntry(base, pdyn, DT_SYMENT);
    table.entsz = (dyn == NULL) ? 0 : dyn->d_un.d_val;

    table.entries = computeNumSymbols(base, pdyn, table);
    return table;
}

struct strTab getStrTab(ElfW(Addr) base, const ElfW(Phdr) * pdyn) {
    struct strTab table;
    const ElfW(Dyn) * dyn;

    dyn = getDynEntry(base, pdyn, DT_STRTAB);
    table.tab = (dyn == NULL) ? NULL : (char *)dyn->d_un.d_ptr;
    dyn = getDynEntry(base, pdyn, DT_STRSZ);
    table.size = (dyn == NULL) ? 0 : dyn->d_un.d_val;
    return table;
}

static void *getGOTEntry(ElfW(Addr) base, jmpRelTab jmprel, symTab symtab, strTab strtab, const char *symname) {
    ElfW(Rela) * rela;
    ElfW(Rela) * relaend;

    relaend = (ElfW(Rela) *)((char *)jmprel.tab + jmprel.size);
    for (rela = jmprel.tab; rela < relaend; rela++) {
        uint32_t relsymidx;
        char *relsymname;
        relsymidx = ELF64_R_SYM(rela->r_info);
        relsymname = strtab.tab + symtab.tab[relsymidx].st_name;
        if (strcmp(symname, relsymname) == 0) {
            return (void *)(base + rela->r_offset);
        }
    }

    return NULL;
}

// Implementation of SymbolMap

void SymbolMap::Load() {
    Log::Info("Loading symbols from current memory\n");
    dl_iterate_phdr(callback, (void *)this);
    Log::Info("Symbols loaded\n");
}

void SymbolMap::ProcessCB(struct dl_phdr_info *info, size_t size) {
    auto phentsize = getauxval(AT_PHENT);
    auto libName = std::string(info->dlpi_name);
    auto base = info->dlpi_addr;
    const auto dphdr = getDynamicPHDR(info->dlpi_phdr, info->dlpi_phnum, phentsize);
    auto jmprel = getJmpRel(info->dlpi_addr, dphdr);
    auto symtab = getSymTab(info->dlpi_addr, dphdr);
    auto strtab = getStrTab(info->dlpi_addr, dphdr);

    if (libName == getCurrentLibraryName()) {
        // Resolve all symbols that need debug symbols
        ElfW(Rela) *relaend = (ElfW(Rela) *)((char *)jmprel.tab + jmprel.size);

        for (ElfW(Rela) *rela = jmprel.tab; rela < relaend; rela++) {
            uint32_t relsymidx = ELF64_R_SYM(rela->r_info);
            auto relsymname = std::string(strtab.tab + symtab.tab[relsymidx].st_name);
            void (**gotPtr)() = (void (**)())(base + rela->r_offset);

            if (functions.count(relsymname) > 0) {               // Check if we have a debug symbol for it
                void *ref = dlsym(nullptr, relsymname.c_str());  // We check if this symbol actually is not resolvable without debug symbols
                if (ref == nullptr) {                            // If is null, that would give undefined symbol during runtime. So we patch with what we have.
                    Log::Debug("Patching: {}\n", demangle(relsymname));
                    // Depending on how the executable was built, it might be read-only
                    void *page = (void *)((intptr_t)gotPtr & ~(0x1000 - 1));
                    mprotect(page, 0x1000, PROT_READ | PROT_WRITE);
                    *gotPtr = (void (*)())functions[relsymname];
                }
            }
        }
    }
}

SymbolMap::SymbolMap(const SymbolMap &map) {
    this->functions = map.functions;
    this->objects = map.objects;
}

SymbolMap::SymbolMap() {
    // Load symbols from bedrock_server
    auto self = getSelfSymbols();
    for (auto &f : self.functions) {
        functions.insert(f);
    }
    for (auto &o : self.objects) {
        objects.insert(o);
    }
    for (auto &m : self.mangled) {
        mangled.insert(m);
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