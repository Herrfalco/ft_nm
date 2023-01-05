/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/05 20:25:57 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

static parse_dat_t		parse_dat = { 0 };

void				parse_init(void) {
	parse_dat.e_hdr = NULL;
	parse_dat.strs = NULL;
	parse_dat.strs_sz = 0;
}

Elf64_Ehdr			*parse_ehdr(mem_t *mem) {
	const char				ident[] = { 0x7f, 'E', 'L', 'F',
		ELFCLASS64 };

	return ((!parse_dat.e_hdr
		&& (!(parse_dat.e_hdr
			= mem_get(mem, 0, 0, sizeof(Elf64_Ehdr)))
		|| strncmp(ident, (char *)mem->data, 5)))
			? NULL : parse_dat.e_hdr);
}

static int		parse_strs(mem_t *mem, char **strs, uint64_t *strs_sz) {
	uint64_t			i;
	Elf64_Ehdr			*e_hdr;
	Elf64_Shdr			*s_hdr;

	if (!parse_dat.strs) {
		if (!(e_hdr = parse_ehdr(mem))
				|| (i = e_hdr->e_shstrndx) == SHN_UNDEF)
			return (-1);
		else if (i == SHN_XINDEX) {
			if (!(s_hdr = mem_get(mem, e_hdr->e_shoff,
					0, e_hdr->e_shentsize)))
				return (-1);
			i = s_hdr->sh_link;
		}
		if (!(s_hdr = mem_get(mem, e_hdr->e_shoff,
				i, e_hdr->e_shentsize)))
			return (-1);
		if (!(parse_dat.strs = mem_get(mem, s_hdr->sh_offset,
				0, s_hdr->sh_size))
			|| parse_dat.strs[s_hdr->sh_size - 1])
			return (-1);
		parse_dat.strs_sz = s_hdr->sh_size;
	}
	*strs = parse_dat.strs;
	*strs_sz = parse_dat.strs_sz;
	return (0);
}

char				*parse_sname(mem_t *mem, uint64_t idx) {
	uint64_t			strs_sz;
	Elf64_Ehdr			*e_hdr;
	Elf64_Shdr			*s_hdr;
	char				*strs;

	if (parse_strs(mem, &strs, &strs_sz)
		|| !(e_hdr = parse_ehdr(mem))
		|| !(s_hdr = mem_get(mem, e_hdr->e_shoff, idx,
			e_hdr->e_shentsize))
		|| s_hdr->sh_name > strs_sz - 1)
		return (NULL);
	return (strs + s_hdr->sh_name);
}

Elf64_Shdr			*parse_shdr(mem_t *mem, const char *name) {
	uint64_t			i = 0, j, strs_sz;
	Elf64_Ehdr			*e_hdr;
	Elf64_Shdr			*s_hdr;
	char				*strs;

	if (!(e_hdr = parse_ehdr(mem))
		|| parse_strs(mem, &strs, &strs_sz))
		return (NULL);
	do {
		for (; i < strs_sz && strs[i]; ++i);
		if (++i > strs_sz - 1)
			return (NULL);
	} while (strcmp(strs + i, name));
	for (j = 0; j < e_hdr->e_shnum; ++j) {
		if (!(s_hdr = mem_get(mem, e_hdr->e_shoff,
				j, e_hdr->e_shentsize)))
			return (NULL);
		if (s_hdr->sh_name == i)
			return (mem_get(mem, s_hdr->sh_offset,
					0, s_hdr->sh_size) ? s_hdr : NULL);
	}
	return (NULL);
}
