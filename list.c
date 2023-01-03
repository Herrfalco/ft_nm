/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 09:18:40 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

void		*get_mem(void *mem, uint64_t mem_sz, uint64_t offset, uint64_t idx, uint64_t size);

int			list_init(list_t *list, void *mem, uint64_t mem_sz, Elf64_Shdr *s_strtab) {
	list->str_sz = s_strtab->sh_size;
	if (!(list->str = get_mem(mem, mem_sz,
			s_strtab->sh_offset, 0, s_strtab->sh_size))
			|| list->str[list->str_sz - 1])
		return (-1);
	list->sz = 0;
	list->cap = BASE_SZ;
	if (!(list->data = malloc(list->cap * sizeof(Elf64_Sym *))))
		return (-1);
	return (0);
}

void		list_free(list_t *list) {
	free(list->data);
}

int			list_push(list_t *list, Elf64_Sym *sym) {
	Elf64_Sym		**new;
	uint64_t		i;

	if (list->sz == list->cap) {
		list->cap *= 2;
		if (!(new = malloc(list->cap * sizeof(Elf64_Sym *))))
			return (-1);
		for (i = 0; i < list->sz; ++i)
			new[i] = list->data[i];
		free(list->data);
		list->data = new;
	}
	list->data[list->sz++] = sym;
	return (0);
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
		printf(" %s\n", list->str + list->data[i]->st_name);
	}
	return (0);
}
