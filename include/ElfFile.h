#ifndef ELF_FILE_H
#define ELF_FILE_H

#include <vector>
#include <elf.h>
#include <cstring>
#include <iostream>



class ElfFile {
public:
    ElfFile(const std::string& filePath);
    ~ElfFile();

    bool isValid() const;
    void displayElfHeader() const;
    void displayProgramHeaders() const;
    void displaySectionHeaders() const;

private:
    std::string filePath;
    Elf64_Ehdr elfHeader;
    std::vector<Elf64_Phdr> programHeaders;
    std::vector<Elf64_Shdr> sectionHeaders;
    std::vector<char> shstrtab; // Container for the section header string table

    bool readHeaders();
    const char* getMachineName(unsigned int e_machine) const;
    std::string getSectionName(uint32_t nameOffset) const;
};


#endif