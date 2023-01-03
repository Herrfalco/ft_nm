/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:15:34 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 17:58:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

Elf64_Ehdr			*parse_ehdr(mem_t *mem) {
	static Elf64_Ehdr		*e_hdr = NULL;
	const char				ident[] = { 0x7f, 'E', 'L', 'F',
		ELFCLASS64 };

	return ((!e_hdr
		&& (!(e_hdr = mem_get(mem, 0, 0, sizeof(Elf64_Ehdr)))
		|| strncmp(ident, (char *)mem->data, 5)))
		? NULL : e_hdr);
}

Elf64_Shdr			*parse_shdr(mem_t *mem, const char *name) {
	uint64_t			i, idx;
	Elf64_Ehdr			*e_hdr;
	Elf64_Shdr			*s_hdr;
	char				*strs;

	if (!(e_hdr = parse_ehdr(mem))
			|| (idx = e_hdr->e_shstrndx) == SHN_UNDEF)
		return (NULL);
	else if (idx == SHN_XINDEX) {
		if (!(s_hdr = mem_get(mem, e_hdr->e_shoff,
				0, e_hdr->e_shentsize)))
			return (NULL);
		idx = s_hdr->sh_link;
	}
	if (!(s_hdr = mem_get(mem, e_hdr->e_shoff,
			idx, e_hdr->e_shentsize)))
		return (NULL);
	if (!(strs = mem_get(mem, s_hdr->sh_offset,
			0, s_hdr->sh_size)))
		return (NULL);
	idx = 0;
	do {
		for (; idx < s_hdr->sh_size && strs[idx]; ++idx);
		if (++idx >= s_hdr->sh_size)
			return (NULL);
	} while (strncmp(strs + idx, name, s_hdr->sh_size - idx));
	for (i = 0; i < e_hdr->e_shnum; ++i) {
		if (!(s_hdr = mem_get(mem, e_hdr->e_shoff,
				i, e_hdr->e_shentsize)))
			return (NULL);
		if (s_hdr->sh_name == idx)
			return (mem_get(mem, s_hdr->sh_offset,
					0, s_hdr->sh_size) ? s_hdr : NULL);
	}
	return (NULL);
}
