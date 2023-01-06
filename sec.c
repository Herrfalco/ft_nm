/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 18:57:26 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sec.h"

static sec_dat_t		sec_dat = { 0 };

// Need further check (endianness...)
static err_t			get_ehdr(Elf64_Ehdr **e_hdr) {
	const char				ident[] = { 0x7f, 'E', 'L', 'F',
		ELFCLASS64 };

	return ((!(*e_hdr
			= mem_get(sec_dat.mem, 0, 0, sizeof(Elf64_Ehdr)))
		|| strncmp(ident, (char *)sec_dat.mem->data, 5))
			? E_EHDR : E_NO);
}

static err_t			get_names(Elf64_Ehdr *e_hdr) {
	uint64_t			idx;
	Elf64_Shdr			*s_hdr;

	if ((idx = e_hdr->e_shstrndx) == SHN_UNDEF)
		return (E_SYM);
	else if (idx == SHN_XINDEX) {
		if (!(s_hdr = mem_get(sec_dat.mem, sec_dat.offset, 0,
				sec_dat.ent_sz)))
			return (E_OOB);
		idx = s_hdr->sh_link;
	}
	if (!(s_hdr = mem_get(sec_dat.mem, sec_dat.offset,
			idx, sec_dat.ent_sz)))
		return (E_OOB);
	if (!(sec_dat.names = mem_get(sec_dat.mem,
			s_hdr->sh_offset, 0, s_hdr->sh_size))
		|| sec_dat.names[s_hdr->sh_size - 1])
		return (E_OOB);
	sec_dat.names_sz = s_hdr->sh_size;
	return (E_NO);
}

err_t				sec_init(mem_t *mem) {
	err_t			err;
	Elf64_Ehdr		*e_hdr;

	sec_dat.mem = mem;
	if ((err = get_ehdr(&e_hdr)))
		return (err);
	sec_dat.offset = e_hdr->e_shoff;
	sec_dat.ent_sz = e_hdr->e_shentsize;
	sec_dat.ent_nb = e_hdr->e_shnum;
	return (get_names(e_hdr));
}

Elf64_Shdr			*sec_from_idx(uint64_t idx) {
	if (idx >= sec_dat.ent_nb)
		return (NULL);
	return (mem_get(sec_dat.mem, sec_dat.offset,
		idx, sec_dat.ent_sz));
}

char				*sec_name(Elf64_Shdr *s_hdr) {
	if (s_hdr->sh_name > sec_dat.names_sz - 1)
		return (NULL);
	return (sec_dat.names + s_hdr->sh_name);
}

Elf64_Shdr			*sec_from_name(const char *name) {
	uint64_t			i;
	Elf64_Shdr			*s_hdr;
	char				*s_name;

	for (i = 0; i < sec_dat.ent_nb; ++i) {
		if (!(s_hdr = sec_from_idx(i)))
			return (NULL);
		if (!(s_name = sec_name(s_hdr)))
			return (NULL);
		if (!strcmp(s_name, name))
			return (s_hdr);
	}
	return (NULL);
}

/*
char				*sec_type_from_idx(uint64_t idx) {
		} else if ((sec_name = parse_sec_name_from_idx(
					list->syms[i]->st_shndx))) {
			if (!strlen(sec_name))
				printf(" U");
			else if (!strcmp(sec_name, ".text")
					|| !strcmp(sec_name, ".plt")
					|| !strcmp(sec_name, ".init")
					|| !strcmp(sec_name, ".fini"))
				printf(glob ? " T" : " t");
			else if (!strcmp(sec_name, ".bss")
					|| !strcmp(sec_name, ".tbss"))
				printf(glob ? " B" : " b");
			else if (!strncmp(sec_name, ".data", 5)
					|| !strcmp(sec_name, ".ctors")
					|| !strcmp(sec_name, ".dynamic")
					|| !strncmp(sec_name, ".got", 4)
					|| !strcmp(sec_name, ".init_array")
					|| !strcmp(sec_name, ".fini_array"))
				printf(glob ? " D" : " d");
			else if (!strncmp(sec_name, ".rodata", 7)
					|| !strncmp(sec_name, ".note", 5)
					|| !strcmp(sec_name, ".gcc_except_table")
					|| !strcmp(sec_name, ".interp")
					|| !strcmp(sec_name, ".dynsym")
					|| !strcmp(sec_name, ".dynstr")
					|| !strcmp(sec_name, ".gnu")
					|| !strcmp(sec_name, ".rela")
					|| !strncmp(sec_name, ".eh_frame", 8))
				printf(glob ? " R" : " r");
			else if (!strcmp(sec_name, ".comment"))
				printf(glob ? " N" : " n");
			else {
				printf(" %s ", sec_name);
//				return (-1);
			}
}
*/
