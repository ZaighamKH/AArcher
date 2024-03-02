#include "ElfFile.h"

#include <fstream>
#include <iomanip>

//-----------------------------------------------------------------------------------------------------
ElfFile::ElfFile(const std::string& filePath) : filePath(filePath)
{
    readHeaders();
}

//-----------------------------------------------------------------------------------------------------
ElfFile::~ElfFile() 
{
}

//-----------------------------------------------------------------------------------------------------
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

    // Read section headers
    elfFile.seekg(elfHeader.e_shoff, std::ios::beg);
    for (int i = 0; i < elfHeader.e_shnum; ++i) {
        Elf64_Shdr shdr;
        elfFile.read(reinterpret_cast<char*>(&shdr), sizeof(Elf64_Shdr));
        if (!elfFile) {
            std::cerr << "Failed to read section header " << i << " from: " << filePath << std::endl;
            return false;
        }
        sectionHeaders.push_back(shdr);
    }

    // Load Section Header String Table
    // Assuming elfHeader.e_shstrndx holds the section header string table index
    const Elf64_Shdr& shstrtabSec = sectionHeaders[elfHeader.e_shstrndx];
    
    elfFile.seekg(shstrtabSec.sh_offset, std::ios::beg);
    shstrtab.resize(shstrtabSec.sh_size);
    elfFile.read(&shstrtab[0], shstrtabSec.sh_size);

    elfFile.close();
    return true;
}

//-----------------------------------------------------------------------------------------------------
void ElfFile::displayElfHeader() const 
{
    std::cout << "ELF Header Information:" << std::endl;
    std::cout << "Type: " << elfHeader.e_type << std::endl;
    std::cout << "Machine: " << elfHeader.e_machine << " (" << getMachineName(elfHeader.e_machine) << ")" <<std::endl;
    std::cout << "Version: " << elfHeader.e_version << std::endl;
    std::cout << "Entry point address: 0x" << std::hex << elfHeader.e_entry << std::endl;
    std::cout << "Program header offset: " << elfHeader.e_phoff << std::endl;
    std::cout << "Section header offset: " << elfHeader.e_shoff << std::endl;
    std::cout << "---------------------------------" << std::endl;
}

//-----------------------------------------------------------------------------------------------------
void ElfFile::displayProgramHeaders() const
{
        size_t phdr_count = programHeaders.size();
        std::cout << "PROGRAM HEADERS:" << std::endl;
        std::cout << "Total number of prgram headers: "<< phdr_count << std::endl;
        std::cout << "---------------------------------" << std::endl;
        for (size_t i = 0; i < phdr_count; ++i) {
            const Elf64_Phdr &ph = programHeaders[i];
            std::cout << "Header " << i << ":" << std::endl;
            // Display program header information
            std::cout << " Type: " << ph.p_type << std::endl;
            std::cout << " Flags: " << ph.p_flags << std::endl;
            std::cout << " Offset: " << ph.p_offset << std::endl;
            std::cout << " File Size " <<ph.p_filesz << std::endl;
            // Add more fields as needed
            std::cout << "---------------------------------" << std::endl;
        }
}

//-----------------------------------------------------------------------------------------------------
void ElfFile::displaySectionHeaders() const {

    std::cout << "Number of Section Headers: " << sectionHeaders.size() << std::endl;
    std::cout << "Executable Sections:" << std::endl;
    for (const auto& sh : sectionHeaders) {
        if (sh.sh_flags & SHF_EXECINSTR) { // Check if the section contains executable instructions
            std::cout << "Section Name: " << getSectionName(sh.sh_name) << std::endl; // Implement this function to retrieve the section name using sh.sh_name
            std::cout << "  Type: " << sh.sh_type << std::endl;
            std::cout << "  Flags: " << sh.sh_flags << std::endl;
            std::cout << "  Address: 0x" 
                      << std::setw(16) << std::setfill('0') << std::hex << sh.sh_addr << std::dec << std::endl;
            std::cout << "  Offset: " << sh.sh_offset << std::endl;
            std::cout << "  Size: " << sh.sh_size << " bytes" << std::endl;
            std::cout << "---------------------------------"  << std::endl;
        }
    }
}
//-----------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------
std::string ElfFile::getSectionName(uint32_t nameOffset) const {
    if (shstrtab.empty()) {
        std::cerr << "Section header string table not loaded." << std::endl;
        return "";
    }
    if (nameOffset >= shstrtab.size()) {
        std::cerr << "Section name offset is out of bounds." << std::endl;
        return "";
    }
    return std::string(&shstrtab[nameOffset]);
}