/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/07 01:11:09 by fcadet           ###   ########.fr       */
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
