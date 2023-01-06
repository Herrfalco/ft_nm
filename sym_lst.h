/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_lst.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:58:59 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 15:07:45 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYM_LST_H
#define SYM_LST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <elf.h>
#include "sym.h"
#include "mem.h"
#include "sec.h"

typedef struct		sym_lst_s {
	Elf64_Sym		**ents;
	uint64_t		ent_nb;
}					sym_lst_t;

void		sym_lst_free(void);
err_t		sym_lst_init(mem_t *mem);
err_t		sym_lst_sort(void);
err_t		sym_lst_print(void);

#endif // SYM_LST_H
