/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:58:59 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/04 11:16:44 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "mem.h"
#include "parse.h"

typedef struct		list_s {
	mem_t			*mem;
	Elf64_Sym		**syms;
	char			*strs;
	uint64_t		sz;
}					list_t;

int			list_init(list_t *list, mem_t *mem, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab);
void		list_free(list_t *list);
void		list_sort(list_t *list);
int			list_print(list_t *list);
