//AARCH4/ARM64 ELF disassambler "AArcher"
//Written by: Muhammad Zaigham Khan
//Created for self-learning purposes

#include "ElfFile.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ELF file>" << std::endl;
        return 1;
    }

    ElfFile elfFile(argv[1]);

    elfFile.displayElfHeader();
    elfFile.displayProgramHeaders();
    elfFile.displaySectionHeaders();

    return 0;
}

