#ifndef FILE_READER_H
#define FILE_READER_H

// Function to match known machine IDs to known names
const char* getMachineName(unsigned int e_machine);

// Function to read the ELF header from a file
bool read_elf_header(const char* filePath);

// Function to display the ELF header information, including the architecture
bool display_elf_header_info();

#endif