/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:21:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 17:58:06 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include "list.h"
#include "mem.h"
#include "error.h"
#include "parse.h"

typedef enum		sec_e {
	DATA,
	BSS,
	TEXT,
	DEBUG,
	RODATA,
	STRTAB,
	SYMTAB,
}					sec_t;

static const char	*SEC_STR[] = {
	".data",
	".bss",
	".text",
	".debug",
	".rodata",
	".strtab",
	".symtab",
};

int			main(int argc, char **argv) {
	int					fd = open(argv[1], O_RDONLY);
	mem_t				mem;
	struct stat			statbuf;
	Elf64_Shdr			*s_strtab, *s_symtab;
	list_t				list;

	if (argc < 2)
		return (error(E_ARG));
	if (!fd)
		return (error(E_FOPEN));
	if (fstat(fd, &statbuf))
		return (error_close(E_FOPER, fd));
	mem.sz = statbuf.st_size;
	if ((mem.data = mmap(NULL, mem.sz, PROT_READ,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error_close(E_ALLOC, fd));
	close(fd);
	if (!parse_ehdr(&mem))
		return (error_unmap(E_EHDR, &mem));
	if (!(s_strtab = parse_shdr(&mem, SEC_STR[STRTAB]))
			|| !(s_symtab = parse_shdr(&mem, SEC_STR[SYMTAB])))
		return (error_unmap(E_SYM, &mem));
	if (list_init(&list, &mem, s_strtab, s_symtab))
		return (error_unmap(E_LIST, &mem));
	list_sort(&list);
	list_print(&list);
	list_free(&list);
	munmap(mem.data, statbuf.st_size);
	return (0);
}
