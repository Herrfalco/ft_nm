/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 11:47:54 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 18:40:32 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYM_H
#define SYM_H

#include <elf.h>
#include <string.h>
#include "sec.h"
#include "mem.h"

typedef enum		sym_type_e {
	T_NO,			// "?"
	T_ABS,			// "A"
	T_UI_D,			// "Bb"
	T_COM,			// "C"
	T_I_D,			// "Dd"
	T_I_SD,			// "Gg"
	T_REL,			// "i"
	T_DEB,			// "N"
	T_UNW,			// "p"
	T_RO,			// "Rr"
	T_UI_SD,		// "Ss"
	T_TXT,			// "Tt"
	T_UD,			// "U"
	T_UNIQ,			// "u"
	T_WK_O,			// "V"
	T_WK_O_UD,		// "v"
	T_WK,			// "W"
	T_WK_UD,		// "w"
}					sym_type_t;

typedef struct		sym_dat_s {
	void			*mem;
	uint64_t		offset;
	uint64_t		ent_sz;
	uint64_t		ent_nb;
	char			*names;
	uint64_t		names_sz;
}					sym_dat_t;

err_t			sym_init(mem_t *mem, uint64_t *sym_nb);
Elf64_Sym		*sym_from_idx(uint64_t idx);
char			*sym_name(Elf64_Sym *sym);
Elf64_Shdr		*sym_sec(Elf64_Sym *sym);
int				sym_filt(Elf64_Sym *sym, err_t *err);
sym_type_t		sym_type(Elf64_Sym *sym);
sym_type_t		sym_type_from_sec(Elf64_Sym *sym, err_t *err);

#endif // SYM_H
