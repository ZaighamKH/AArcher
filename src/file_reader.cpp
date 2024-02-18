#include "file_reader.h"

#include <iostream>
#include <fstream>
#include <elf.h>
#include <cstring>

// Global variable to store ELF header; makes it accessible for displaying info
Elf64_Ehdr ehdr;

// Function to map e_machine values to architecture names
const char* getMachineName(unsigned int e_machine) {
    switch (e_machine) {
    case EM_NONE: return "No machine";
    case EM_386: return "Intel 80386";
    case EM_X86_64: return "AMD x86-64";
    case EM_ARM: return "ARM";
    case EM_AARCH64: return "ARM64";
    case EM_MIPS: return "MIPS";
    case EM_PPC: return "PowerPC";
    case EM_PPC64: return "PowerPC64";
    // Add more architectures as needed
    default: return "Unknown";
    }
}

bool read_elf_header(const char* filePath) {

    std::ifstream elfFile(filePath, std::ios::in | std::ios::binary);
    if (!elfFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    elfFile.read(reinterpret_cast<char*>(&ehdr), sizeof(ehdr));
    if (elfFile.gcount() < sizeof(ehdr)) {
        std::cerr << "Failed to read ELF header." << std::endl;
        return false;
    }

    if (std::memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        std::cerr << "Not a valid ELF file." << std::endl;
        return false;
    }

    elfFile.close();
    return true;
}

bool display_elf_header_info() {

    std::cout << "ELF Header Information:" << std::endl;
    std::cout << "Type: " << ehdr.e_type << std::endl;
    std::cout << "Machine: " << ehdr.e_machine << " (" << getMachineName(ehdr.e_machine) << ")" <<std::endl;
    std::cout << "Version: " << ehdr.e_version << std::endl;
    std::cout << "Entry point address: 0x" << std::hex << ehdr.e_entry << std::endl;
    std::cout << "Program header offset: " << ehdr.e_phoff << std::endl;
    std::cout << "Section header offset: " << ehdr.e_shoff << std::endl;

    //Confirm if File is AARCH64 or not
    bool result = (ehdr.e_machine == 183) ? true : false; 

    return result;    
}