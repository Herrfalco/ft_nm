/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:17:07 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/05 20:19:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
#define PARSE_H

#include <elf.h>
#include <string.h>
#include "mem.h"

typedef struct		parse_dat_s {
	Elf64_Ehdr		*e_hdr;
	char			*strs;
	uint64_t		strs_sz;
}					parse_dat_t;

void				parse_init(void);
Elf64_Ehdr			*parse_ehdr(mem_t *mem);
char				*parse_sname(mem_t *mem, uint64_t idx);
Elf64_Shdr			*parse_shdr(mem_t *mem, const char *name);

#endif // PARSE_H
