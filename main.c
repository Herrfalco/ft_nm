/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:21:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/02 17:53:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "list.h"

#include <string.h>

typedef enum		e_err {
	E_ARG,
	E_FOPEN,
	E_FOPER,
	E_ALLOC,
	E_EHDR,
	E_SHDR,
}					t_err;

static int			error(t_err err) {
	const char		*strs[] = {
		"Invalid arguent",
		"Can't open file",
		"File operation failed",
		"Can't allocation memory",
		"Can't get program header",
		"Can't find desired section",
	};

	fprintf(stderr, "Error: %s\n", strs[err]);
	return (err + 1);
}

static void		*get_mem(void *mem, uint64_t mem_sz, uint64_t offset, uint64_t idx, uint64_t size) {
	if (!size || offset + (idx + 1) * size > mem_sz)
		return (NULL);
	return (mem + offset + idx * size);
}

static Elf64_Shdr		*find_strtab(void *mem, struct stat *statbuf, Elf64_Ehdr *e_hdr) {
	uint64_t			i, idx;
	Elf64_Shdr			*s_hdr, *s_shstrtab;
	char				*strs;

	if ((idx = e_hdr->e_shstrndx) == SHN_UNDEF)
		return (NULL);
	else if (idx == SHN_XINDEX) {
		if (!(s_hdr = get_mem(mem, statbuf->st_size,
				e_hdr->e_shoff, 0, e_hdr->e_shentsize)))
			return (NULL);
		idx = s_hdr->sh_link;
	}
	if (!(s_shstrtab = get_mem(mem, statbuf->st_size,
			e_hdr->e_shoff, idx, e_hdr->e_shentsize)))
		return (NULL);
	if (!(strs = get_mem(mem, statbuf->st_size,
			s_shstrtab->sh_offset, 0, s_shstrtab->sh_size)))
		return (NULL);
	idx = 0;
	do {
		for (; idx < s_shstrtab->sh_size && strs[idx]; ++idx);
		if (++idx >= s_shstrtab->sh_size)
			return (NULL);
	} while (strncmp(strs + idx, ".strtab",
		s_shstrtab->sh_size - idx));
	for (i = 0; i < e_hdr->e_shnum; ++i) {
		if (!(s_hdr = get_mem(mem, statbuf->st_size,
				e_hdr->e_shoff, i, e_hdr->e_shentsize)))
			return (NULL);
		if (s_hdr->sh_name == idx)
			return (s_hdr);
	}
	return (NULL);
}

int			disp_symtab(void *mem, struct stat *statbuf, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab) {
	uint64_t			idx, size;
	Elf64_Sym			*sym;
	list_t				list;
	char				*sym_str;


	if (!s_symtab->sh_entsize
			|| !(sym_str = get_mem(mem, statbuf->st_size,
				s_strtab->sh_offset, 0, s_strtab->sh_size))
			|| list_init(&list))
		return (-1);
	size = s_symtab->sh_size / s_symtab->sh_entsize;
	for (idx = 0; idx < size; ++idx) {
		if (!(sym = get_mem(mem, statbuf->st_size,
				s_symtab->sh_offset, idx,
				s_symtab->sh_entsize))) {
			list_free(&list);
			return (-1);
		}
		if (sym->st_name && sym->st_info != STT_FILE) {
			if (sym->st_name > s_strtab->sh_size
					|| sym_str[s_strtab->sh_size - 1]) {
				list_free(&list);
				return (-1);
			}
			list_push(&list, sym_str + sym->st_name);
		}
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
	if (fstat(fd, &statbuf)) {
		close(fd);
		return (error(E_FOPER));
	}
	if ((mem = mmap(NULL, statbuf.st_size, PROT_READ,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		close(fd);
		return (error(E_ALLOC));
	}
	close(fd);
	if (!(e_hdr = get_mem(mem, statbuf.st_size, 0, 0,
			sizeof(Elf64_Ehdr)))) {
		munmap(mem, statbuf.st_size);
		return (error(E_EHDR));
	}
	if (!(s_strtab = find_strtab(mem, &statbuf, e_hdr))) {
		munmap(mem, statbuf.st_size);
		return (error(E_SHDR));
	}
	for (i = 0; i < e_hdr->e_shnum; ++i) {
		if (!(s_symtab = get_mem(mem, statbuf.st_size,
				e_hdr->e_shoff, i, e_hdr->e_shentsize))) {
			munmap(mem, statbuf.st_size);
			return (error(E_SHDR));
		}
		if (s_symtab->sh_type == SHT_SYMTAB) {
			if (disp_symtab(mem, &statbuf,
					s_strtab, s_symtab)) {
				munmap(mem, statbuf.st_size);
				return (error(E_SHDR));
			}
		}
	}
	munmap(mem, statbuf.st_size);
	return (0);
}
