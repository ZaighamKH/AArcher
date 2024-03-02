#include "ElfFile.h"

//#include <iostream>
#include <fstream>
//#include <elf.h>
//#include <cstring>

ElfFile::ElfFile(const std::string& filePath) : filePath(filePath)
{
    readHeaders();
}

ElfFile::~ElfFile() 
{
}

const char* ElfFile::getMachineName(unsigned int e_machine) const
{
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

void ElfFile::displayElfHeader() const 
{
    std::cout << "ELF Header Information:" << std::endl;
    std::cout << "Type: " << elfHeader.e_type << std::endl;
    std::cout << "Machine: " << elfHeader.e_machine << " (" << getMachineName(elfHeader.e_machine) << ")" <<std::endl;
    std::cout << "Version: " << elfHeader.e_version << std::endl;
    std::cout << "Entry point address: 0x" << std::hex << elfHeader.e_entry << std::endl;
    std::cout << "Program header offset: " << elfHeader.e_phoff << std::endl;
    std::cout << "Section header offset: " << elfHeader.e_shoff << std::endl;
}

void ElfFile::displayProgramHeaders() const
{
        std::cout << "Program Headers:" << std::endl;
        for (size_t i = 0; i < programHeaders.size(); ++i) {
            const Elf64_Phdr &ph = programHeaders[i];
            std::cout << "Header " << i << ":" << std::endl;
            // Display program header information
            std::cout << "  Type: " << ph.p_type << std::endl;
            std::cout << "Flags: " <<ph.p_flags << std::endl;
            // Add more fields as needed
        }
}

bool ElfFile::readHeaders() 
{
    std::ifstream elfFile(filePath, std::ios::binary);
    if (!elfFile.is_open()) 
    {
        std::cerr << "Failed to open ELF file: " << filePath << std::endl;
        return false;
    }

    // Read ELF header
    elfFile.read(reinterpret_cast<char*>(&elfHeader), sizeof(Elf64_Ehdr));
    if (!elfFile)
    {
        std::cerr << "Failed to read ELF header from: " << filePath << std::endl;
        return false;
    }

    // Read program headers
    elfFile.seekg(elfHeader.e_phoff, std::ios::beg);
    for (int i = 0; i < elfHeader.e_phnum; ++i) 
    {
        Elf64_Phdr phdr;
        elfFile.read(reinterpret_cast<char*>(&phdr), sizeof(Elf64_Phdr));
        if (!elfFile) 
        {
            std::cerr << "Failed to read program header " << i << " from: " << filePath << std::endl;
            return false;
        }
        programHeaders.push_back(phdr);
    }
    elfFile.close();
    return true;
}
