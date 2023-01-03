/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 12:05:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

int			list_init(list_t *list, mem_t *mem, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab) {
	uint64_t		i, sym_nb;
	Elf64_Sym		*sym;
	char			*strs;
		
	if (!(strs = mem_get(mem, s_strtab->sh_offset,
			0, s_strtab->sh_size))
			|| strs[s_strtab->sh_size - 1])
		return (-1);
	if (!s_symtab->sh_entsize)
		return (-1);
	sym_nb = s_symtab->sh_size / s_symtab->sh_entsize;
	// allocation
	for (i = 0; i < sym_nb; ++i) {
		if (!(sym = mem_get(mem, s_symtab->sh_offset,
				i, s_symtab->sh_entsize))
				|| sym->st_name > s_strtab->sh_size - 1)
			return (-1);
		// initialization
	}
	return (0);
}

void		list_free(list_t *list) {
	free(list->data);
}

int			list_sort(list_t *list) {
	uint64_t		i, j, sz;
	char			*s1, *s2;
	Elf64_Sym		*tmp;

	for (sz = list->sz; sz > 1; --sz) {
		for (i = 0; i + 1 < sz; ++i) {
			if (list->data[i]->st_name > list->str_sz
					|| list->data[i + 1]->st_name > list->str_sz)
				return (-1);
			s1 = list->str + list->data[i]->st_name;
			s2 = list->str + list->data[i + 1]->st_name;
			for (j = 0; s1[j] && s1[j] == s2[j]; ++j);
			if (s1[j] > s2[j]) {
				tmp = list->data[i];
				list->data[i] = list->data[i + 1];
				list->data[i + 1] = tmp;
			}
		}
	}
	return (0);
}

static void	hex_print(uint64_t val, uint8_t size) {
	if ((size - 4) / 4)
		hex_print(val / 16, size - 4);
	printf("%c", (char)(val % 16
		+ (val % 16 < 10 ? '0' : 'a' - 10)));
}

int			list_print(list_t *list) {
	uint64_t		i;

	for (i = 0; i < list->sz; ++i) {
		if (i >= list->str_sz)
			return (-1);
		if (!list->data[i]->st_value)
			printf("                ");
		else
			hex_print(list->data[i]->st_value, 64);
		if (list->data[i]->st_info == STT_NOTYPE)
			printf(" U");
		printf(" %s\n", list->str + list->data[i]->st_name);
	}
	return (0);
}
