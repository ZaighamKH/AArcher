#include "file_reader.h"

#include <iostream>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ELF file>" << std::endl;
        return 1;
    }

    if (read_elf_header(argv[1])) {
        bool aarch_check = display_elf_header_info();
        if (!aarch_check) {
            std::cerr << "File is not AARCH64 Binary!" << std::endl;
            return 1;
        } 
    } else {
        std::cerr << "Failed to read ELF header." << std::endl;
        return 1;
    }

    return 0;
}