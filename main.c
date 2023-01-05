/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:21:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/05 16:48:26 by fcadet           ###   ########.fr       */
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

static int			treat_file(char *file, uint8_t multi) {
	int					fd = open(file, O_RDONLY);
	mem_t				mem;
	struct stat			statbuf;
	Elf64_Shdr			*s_strtab, *s_symtab;
	list_t				list;

	if (fd < 0)
		return (error(E_FOPEN, file));
	if (fstat(fd, &statbuf))
		return (error_close(E_FOPER, fd, file));
	mem.sz = statbuf.st_size;
	if ((mem.data = mmap(NULL, mem.sz, PROT_READ,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error_close(E_ALLOC, fd, file));
	close(fd);
	parse_init();
	if (!parse_ehdr(&mem))
		return (error_unmap(E_EHDR, &mem, file));
	if (!(s_strtab = parse_shdr(&mem, ".strtab"))
		|| !(s_symtab = parse_shdr(&mem, ".symtab")))
		return (error_unmap(E_SYM, &mem, file));
	if (list_init(&list, &mem, s_strtab, s_symtab))
		return (error_unmap(E_LIST, &mem, file));
	list_sort(&list);
	if (multi)
		printf("\n%s:\n", file);
	if (list_print(&list))
		return (error_unmap(E_PLIST, &mem, file));
	list_free(&list);
	munmap(mem.data, statbuf.st_size);
	return (E_NO);
}

int			main(int argc, char **argv) {
	int		i, ret = 0;

	if (argc == 1)
		return (treat_file("a.out", 0));
	for (i = 1; i < argc; ++i)
		ret |= treat_file(argv[i], argc > 2);
	return (ret);
}
