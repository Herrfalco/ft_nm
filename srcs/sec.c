/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sec.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 13:32:00 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/sec.h"

static sec_dat_t		sec_dat = { 0 };

// Need further check (endianness...)
static err_t			get_ehdr(void **e_hdr) {
	const char				ident[] = { 0x7f, 'E', 'L', 'F' };
	char					*e_ident;

	if (!(e_ident = mem_get(sec_dat.mem, 0, 0, 5))
		|| strncmp(ident, e_ident, 4)
		|| arch_set(e_ident[4]))
		return (E_EHDR);
	return ((!(*e_hdr = mem_get(sec_dat.mem, 0, 0, arch_is_64()
		? sizeof(Elf64_Ehdr) : sizeof(Elf32_Ehdr)))
		? E_EHDR : E_NO));
}

static err_t			get_names(void *e_hdr) {
	uint64_t			idx;
	void				*s_hdr;

	if ((idx = arch_ehdr(e_hdr, EF_SHSTRNDX)) == SHN_UNDEF)
		return (E_SYM);
	else if (idx == SHN_XINDEX) {
		if (!(s_hdr = mem_get(sec_dat.mem, sec_dat.offset, 0,
				sec_dat.ent_sz)))
			return (E_OOB);
		idx = arch_shdr(s_hdr, SF_LINK);
	}
	if (!(s_hdr = mem_get(sec_dat.mem, sec_dat.offset,
			idx, sec_dat.ent_sz)))
		return (E_OOB);
	if (!(sec_dat.names = mem_get(sec_dat.mem,
			arch_shdr(s_hdr, SF_OFFSET),
			0, arch_shdr(s_hdr, SF_SIZE)))
		|| sec_dat.names[arch_shdr(s_hdr, SF_SIZE) - 1])
		return (E_OOB);
	sec_dat.names_sz = arch_shdr(s_hdr, SF_SIZE);
	return (E_NO);
}

err_t				sec_init(mem_t *mem) {
	err_t			err;
	void			*e_hdr;

	sec_dat.mem = mem;
	if ((err = get_ehdr(&e_hdr)))
		return (err);
	sec_dat.offset = arch_ehdr(e_hdr, EF_SHOFF);
	sec_dat.ent_sz = arch_ehdr(e_hdr, EF_SHENTSIZE);
	sec_dat.ent_nb = arch_ehdr(e_hdr, EF_SHNUM);
	return (get_names(e_hdr));
}

void				*sec_from_idx(uint64_t idx) {
	if (idx >= sec_dat.ent_nb)
		return (NULL);
	return (mem_get(sec_dat.mem, sec_dat.offset,
		idx, sec_dat.ent_sz));
}

char				*sec_name(void *s_hdr) {
	if (arch_shdr(s_hdr, SF_NAME) > sec_dat.names_sz - 1)
		return (NULL);
	return (sec_dat.names + arch_shdr(s_hdr, SF_NAME));
}

void				*sec_from_name(const char *name) {
	uint64_t	i;
	void		*s_hdr;
	char		*s_name;

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
