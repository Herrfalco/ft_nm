/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sec.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:17:07 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/11 07:54:42 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SEC_H
#define SEC_H

#include <elf.h>
#include "mem.h"
#include "error.h"
#include "arch.h"

typedef struct		sec_dat_s {
	mem_t			*mem;
	uint64_t		offset;
	uint64_t		ent_sz;
	uint64_t		ent_nb;
	char			*names;
	uint64_t		names_sz;
}					sec_dat_t;

err_t				sec_init(mem_t *mem);
void				*sec_from_idx(uint64_t idx);
char				*sec_name(void *s_hdr);
void				*sec_from_name(const char *name);

#endif // SEC_H
