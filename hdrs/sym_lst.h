/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_lst.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:58:59 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 14:45:52 by fcadet           ###   ########.fr       */
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
#include "arch.h"
#include "opts.h"

typedef struct		sym_lst_s {
	void			**ents;
	uint64_t		ent_nb;
}					sym_lst_t;

void		sym_lst_free(void);
err_t		sym_lst_init(mem_t *mem);
err_t		sym_lst_sort(void);
err_t		sym_lst_print(void);

#endif // SYM_LST_H
