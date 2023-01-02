/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/02 16:59:55 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

int			list_init(list_t *list) {
	list->sz = 0;
	list->cap = BASE_SZ;
	if (!(list->data = malloc(list->cap * sizeof(char *))))
		return (-1);
	return (0);
}

void		list_free(list_t *list) {
	free(list->data);
}

int			list_push(list_t *list, char *str) {
	char			**new;
	uint64_t		i;

	if (list->sz == list->cap) {
		list->cap *= 2;
		if (!(new = malloc(list->cap * sizeof(char *))))
			return (-1);
		for (i = 0; i < list->sz; ++i)
			new[i] = list->data[i];
		free(list->data);
		list->data = new;
	}
	list->data[list->sz++] = str;
	return (0);
}

void		list_populate(list_t *list, char **str, uint64_t sz) {
	uint64_t		i;

	for (i = 0; i < sz; ++i)
		list_push(list, str[i]);
}

void		list_sort(list_t *list) {
	uint64_t		i, j, sz;
	char			*tmp;

	for (sz = list->sz; sz > 1; --sz) {
		for (i = 0; i + 1 < sz; ++i) {
			for (j = 0; list->data[i][j]
				&& list->data[i][j] == list->data[i + 1][j];
				++j);
			if (list->data[i][j] > list->data[i + 1][j]) {
				tmp = list->data[i];
				list->data[i] = list->data[i + 1];
				list->data[i + 1] = tmp;
			}
		}
	}
}

void		list_print(list_t *list) {
	uint64_t		i;

	for (i = 0; i < list->sz; ++i)
		printf("%s\n", list->data[i]);
}

/*
int			main(int argc, char **argv) {
	list_t		list;

	list_init(&list);
	list_populate(&list, argv, argc);
	list_sort(&list);
	list_print(&list);
	list_free(&list);
	return (0);
}
*/
