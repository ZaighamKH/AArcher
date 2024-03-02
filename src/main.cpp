#include "ElfFile.h"

//#include <iostream>
//#include <cstring>

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <ELF file>" << std::endl;
        return 1;
    }

    ElfFile elfFile(argv[1]);

    elfFile.displayElfHeader();
    elfFile.displayProgramHeaders();

    return 0;
}

