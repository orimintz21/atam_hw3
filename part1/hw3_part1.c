#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <syscall.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "elf64.h"

#define ET_NONE 0 // No file type
#define ET_REL 1  // Relocatable file
#define ET_EXEC 2 // Executable file
#define ET_DYN 3  // Shared object file
#define ET_CORE 4 // Core file

/* symbol_name		- The symbol (maybe function) we need to search for.
 * exe_file_name	- The file where we search the symbol in.
 * error_val		- If  1: A global symbol was found, and defined in the given executable.
 * 			- If -1: Symbol not found.
 *			- If -2: Only a local symbol was found.
 * 			- If -3: File is not an executable.
 * 			- If -4: The symbol was found, it is global, but it is not defined in the executable.
 * return value		- The address which the symbol_name will be loaded to, if the symbol was found and is global.
 */

Elf64_Shdr *getStringTableHeader(int fd, Elf64_Ehdr *elf_header, Elf64_Shdr *section_header, int *section_string_index);

Elf64_Shdr *getSymbolTableHeader(int fd, Elf64_Ehdr *elf_headerm, Elf64_Shdr *section_header);

unsigned long find_symbol(char *symbol_name, char *exe_file_name, int *error_val)
{
	// TODO: Implement.
	if (!symbol_name || !exe_file_name)
	{
		*error_val = -ET_REL;
		return 1;
	}
	int fd = open(exe_file_name, O_RDONLY);
	if (fd < 0)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	Elf64_Ehdr elf_header;
	if (read(fd, &elf_header, sizeof(elf_header)) != sizeof(elf_header))
	{
		*error_val = -ET_DYN;
		return 1;
	}
	if (elf_header.e_type != ET_EXEC)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	Elf64_Shdr *section_header = malloc(elf_header.e_shentsize * elf_header.e_shnum);
	if (!section_header)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	if (lseek(fd, elf_header.e_shoff, SEEK_SET) < 0)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	if (read(fd, section_header, elf_header.e_shentsize * elf_header.e_shnum) != elf_header.e_shentsize * elf_header.e_shnum)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	Elf64_Shdr section_header_string_table = section_header[elf_header.e_shstrndx];
	int section_string_index = 0;
	Elf64_Shdr *string_table_header = getStringTableHeader(fd, &elf_header, section_header, &section_string_index);
	if (!string_table_header)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	Elf64_Shdr *symbol_table_header = getSymbolTableHeader(fd, &elf_header, section_header);
	if (!symbol_table_header)
	{
		*error_val = -ET_DYN;
		return 1;
	}
	int symtab_entries_num = symbol_table_header->sh_size / symbol_table_header->sh_entsize;
	for (int i = 0; i < symtab_entries_num; i++)
	{
	}

	free(section_header);
	close(fd);
	return 0;
}

Elf64_Shdr *getStringTableHeader(int fd, Elf64_Ehdr *elf_header, Elf64_Shdr *section_header, int *section_string_index)
{
	Elf64_Shdr section_header_string_table = section_header[elf_header->e_shstrndx];
	char *section_string_table = malloc(section_header_string_table.sh_size);
	if (!section_string_table)
	{
		return NULL;
	}
	if (lseek(fd, section_header_string_table.sh_offset, SEEK_SET) < 0)
	{
		return NULL;
	}
	if (read(fd, section_string_table, section_header_string_table.sh_size) != section_header_string_table.sh_size)
	{
		return NULL;
	}
	for (int i = 0; i < elf_header->e_shnum; i++)
	{
		if (strcmp(section_string_table + section_header[i].sh_name, ".strtab") == 0)
		{
			*section_string_index = i;
			return &section_header[i];
		}
	}
	return NULL;
}

// TODO: psodo
/*
unsigned long find_symbol(char *symbol_name, char *exe_file_name, int *error_val){
	elf header <- beginning of file
	if elf_header.type != executable or header is null:
		error not executable, return;
	all_sections_array = elf_header + sh_offset ("All sections array now contains all sections")
	all_sections_array[elf_header.section_string_table] -> section_string_table ("This table contains the strings of all sections")
	Find string table ("The table which contains the strings of all symbols"):
		Iterate over all sections array:
			For each section, find the offset name
			section name = section_string_table + offset name
			if section name == strtab then this the section, return it
	Find symbol table: the same as above, just compare to symtab
	Iterate over symbol table entries:
		For each symbol, find the symbol name offset
		actual name = string_table + offset
		if actual_name == *symbol_name:
			Check st_info field of the symbol
			if st_info is global, return the symbol offset
*/

int main(int argc, char *const argv[])
{
	int err = 0;
	unsigned long addr = find_symbol(argv[1], argv[2], &err);

	if (addr > 0)
		printf("%s will be loaded to 0x%lx\n", argv[1], addr);
	else if (err == -2)
		printf("%s is not a global symbol! :(\n", argv[1]);
	else if (err == -1)
		printf("%s not found!\n", argv[1]);
	else if (err == -3)
		printf("%s not an executable! :(\n", argv[2]);
	else if (err == -4)
		printf("%s is a global symbol, but will come from a shared library\n", argv[1]);
	return 0;
}