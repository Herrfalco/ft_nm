/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 17:02:38 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

int			list_init(list_t *list, mem_t *mem, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab) {
	uint64_t		i;

	if (!(list->strs = mem_get(mem, s_strtab->sh_offset,
			0, s_strtab->sh_size))
			|| list->strs[s_strtab->sh_size - 1])
		return (-1);
	if (!s_symtab->sh_entsize)
		return (-1);
	list->sz = s_symtab->sh_size / s_symtab->sh_entsize;
	if (!(list->syms = malloc(sizeof(Elf64_Sym *) * list->sz)))
		return (-1);
	for (i = 0; i < list->sz; ++i) {
		if (!(list->syms[i] = mem_get(mem, s_symtab->sh_offset,
					i, s_symtab->sh_entsize))
				|| list->syms[i]->st_name
					> s_strtab->sh_size - 1) {
			free(list->syms);
			return (-1);
		}
	}
	return (0);
}

void		list_free(list_t *list) {
	free(list->syms);
}

int			list_sort(list_t *list) {
	uint64_t		i, j, sz;
	char			*s1, *s2;
	Elf64_Sym		*tmp;

	for (sz = list->sz; sz > 1; --sz) {
		for (i = 0; i + 1 < sz; ++i) {
			s1 = list->strs + list->syms[i]->st_name;
			s2 = list->strs + list->syms[i + 1]->st_name;
			for (j = 0; s1[j] && s1[j] == s2[j]; ++j);
			if (s1[j] > s2[j]) {
				tmp = list->syms[i];
				list->syms[i] = list->syms[i + 1];
				list->syms[i + 1] = tmp;
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
		if (list->syms[i]->st_info == STT_FILE)
			continue;
		if (!list->syms[i]->st_value)
			printf("                ");
		else
			hex_print(list->syms[i]->st_value, 64);
		printf(" %s\n", list->strs + list->syms[i]->st_name);
	}
	return (0);
}
