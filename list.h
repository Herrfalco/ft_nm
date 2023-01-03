/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:58:59 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 16:27:59 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "mem.h"

#define BASE_SZ		128

typedef struct		list_s {
	Elf64_Sym		**syms;
	char			*strs;
	uint64_t		sz;
}					list_t;

int			list_init(list_t *list, mem_t *mem, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab);
void		list_free(list_t *list);
int			list_push(list_t *list, Elf64_Sym *sym);
int			list_sort(list_t *list);
int			list_print(list_t *list);
