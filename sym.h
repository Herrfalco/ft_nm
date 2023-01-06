/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 11:47:54 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 18:19:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYM_H
#define SYM_H

#include <elf.h>
#include <string.h>
#include "sec.h"
#include "mem.h"

typedef enum		sym_type_e {
	T_NO,			// "?"		OK
	T_ABS,			// "A"		OK
	T_UI_D,			// "Bb"		OK
	T_COM,			// "C"		OK
	T_I_D,			// "Dd"		OK
	T_I_SD,			// "Gg"		OK
	T_REF,			// "I"
	T_REL,			// "i"		OK
	T_DEB,			// "N"		OK
	T_RO_2,			// "n"
	T_UNW,			// "p"		OK
	T_RO,			// "Rr"		OK
	T_UI_SD,		// "Ss"		OK
	T_TXT,			// "Tt"		OK
	T_UD,			// "U"		OK
	T_UNIQ,			// "u"		OK
	T_WK_O,			// "V"		OK
	T_WK_O_UD	,	// "v"		OK
	T_WK,			// "W"		OK
	T_WK_UD,		// "w"		OK
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
