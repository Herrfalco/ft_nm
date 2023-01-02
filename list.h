/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:58:59 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/02 17:00:07 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define BASE_SZ		128

typedef struct		list_s {
	char			**data;
	uint64_t		sz;
	uint64_t		cap;
}					list_t;

int			list_init(list_t *list);
void		list_free(list_t *list);
int			list_push(list_t *list, char *str);
void		list_populate(list_t *list, char **str, uint64_t sz);
void		list_sort(list_t *list);
void		list_print(list_t *list);
