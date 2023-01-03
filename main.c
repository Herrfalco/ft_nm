/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:21:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 11:29:45 by fcadet           ###   ########.fr       */
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

int			disp_symtab(void *mem, struct stat *statbuf, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab) {
	uint64_t			idx, size;
	Elf64_Sym			*sym;
	list_t				list;

	if (!s_symtab->sh_entsize
			|| list_init(&list, mem, statbuf->st_size, s_strtab))
		return (-1);
	size = s_symtab->sh_size / s_symtab->sh_entsize;
	for (idx = 0; idx < size; ++idx) {
		if (!(sym = mem_get(mem, s_symtab->sh_offset, idx,
				s_symtab->sh_entsize))) {
			list_free(&list);
			return (-1);
		}
		if (sym->st_name && sym->st_info != STT_FILE)
			list_push(&list, sym);
	}
	list_sort(&list);
	list_print(&list);
	list_free(&list);
	return (0);
}

int			main(int argc, char **argv) {
	uint64_t			i;
	int					fd = open(argv[1], O_RDONLY);
	void				*mem;
	struct stat			statbuf;
	Elf64_Ehdr			*e_hdr;
	Elf64_Shdr			*s_strtab, *s_symtab;

	if (argc < 2)
		return (error(E_ARG));
	if (!fd)
		return (error(E_FOPEN));
	if (fstat(fd, &statbuf))
		return (error_close(E_FOPER, fd));
	if ((mem = mmap(NULL, statbuf.st_size, PROT_READ,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error_close(E_ALLOC, fd));
	close(fd);

	if (!parse_ehdr(mem))
		return (error_unmap(E_EHDR, mem, statbuf.st_size));
	if (!(s_strtab = parse_shdr(mem, ".strtab"))
			|| !(s_symtab = parse_shdr(mem, ".symtab")))
		return (error_unmap(E_SYM, mem, statbuf.st_size));

	if (disp_symtab(mem, &statbuf,
			s_strtab, s_symtab))
		return (error_unmap(E_SHDR, mem, statbuf.st_size));

	munmap(mem, statbuf.st_size);
	return (0);
}
