/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 11:47:02 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 13:32:07 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/sym.h"

static sym_dat_t		sym_dat = { 0 };

err_t			sym_init(mem_t *mem, uint64_t *sym_nb) {
	void		*s_strtab, *s_symtab;
	err_t		err;

	if ((err = sec_init(mem)))
		return (err);
	sym_dat.mem = mem;
	if (!(s_strtab = sec_from_name(".strtab"))
		|| !(s_symtab = sec_from_name(".symtab")))
		return (E_SYM);
	sym_dat.offset = arch_shdr(s_symtab, SF_OFFSET);
	if (!(sym_dat.ent_sz = arch_shdr(s_symtab, SF_ENTSIZE)))
		return (E_SYM);
	sym_dat.ent_nb = arch_shdr(s_symtab, SF_SIZE)
		/ sym_dat.ent_sz;
	*sym_nb = sym_dat.ent_nb;
	sym_dat.names_sz = arch_shdr(s_strtab, SF_SIZE);
	if (!(sym_dat.names = mem_get(mem,
			arch_shdr(s_strtab, SF_OFFSET),
			0, sym_dat.names_sz))
		|| sym_dat.names[sym_dat.names_sz - 1])
		return (E_SYM);
	return (E_NO);
}

void		*sym_from_idx(uint64_t idx) {
	if (idx >= sym_dat.ent_nb)
		return (NULL);
	return (mem_get(sym_dat.mem, sym_dat.offset,
		idx, sym_dat.ent_sz));
}

char			*sym_name(void *sym) {
	if (arch_sym(sym, SMF_NAME) > sym_dat.names_sz - 1)
		return (NULL);
	return (sym_dat.names + arch_sym(sym, SMF_NAME));
}

int				sym_filt(void *sym, err_t *err) {
	uint8_t		info = arch_sym(sym, SMF_INFO),
				type = arch_macro(&info, M_ST_TYPE);
	char		*name;

	if ((*err = !(name = sym_name(sym)) ? E_OOB : E_NO))
		return (-1);
	return ((type == STT_FILE || type == STT_SECTION
		|| !strlen(name)
		|| !strcmp(name, "$d")
		|| !strcmp(name, "$f")
		|| !strcmp(name, "$m")
		|| !strcmp(name, "$p")
		|| !strcmp(name, "$x")) ? 1 : 0);
}

sym_type_t		sym_type(void *sym) {
	uint8_t		info = arch_sym(sym, SMF_INFO),
				type = arch_macro(&info, M_ST_TYPE),
				bind = arch_macro(&info, M_ST_BIND);
	uint16_t	shndx = arch_sym(sym, SMF_SHNDX);

	if (bind == STB_GNU_UNIQUE)
		return (T_UNIQ);
	else if (type == STT_GNU_IFUNC)
		return (T_REL);
	else if (bind == STB_WEAK)
		return (type == STT_OBJECT
			? (shndx == SHN_UNDEF ? T_WK_O_UD : T_WK_O)
			: (shndx == SHN_UNDEF ? T_WK_UD : T_WK));
	switch (shndx) {
		case SHN_UNDEF:
			return (T_UD);
		case SHN_ABS:
			return (T_ABS);
		case SHN_COMMON:
			return (T_COM);
		default:
			return (T_NO);
	}
}

sym_type_t		sym_type_from_sec(void *sym, err_t *err) {
	void			*sec;
	uint64_t		sec_prof;

	if ((*err = (sec = sec_from_idx(arch_sym(sym, SMF_SHNDX)))
		? E_NO : E_OOB))
		return (T_NO);
	if (arch_shdr(sec, SF_TYPE) == SHT_NOBITS)
		return (arch_shdr(sec, SF_FLAGS) & SHF_IA_64_SHORT
			? T_UI_SD : T_UI_D);
	else if (arch_shdr(sec, SF_TYPE) == SHT_IA_64_UNWIND)
		return (T_UNW);
	sec_prof = arch_shdr(sec, SF_FLAGS)
		& (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR);
	switch (sec_prof) {
		case SHF_ALLOC | SHF_EXECINSTR:
			return (T_TXT);
		case SHF_ALLOC | SHF_WRITE:
			return (arch_shdr(sec, SF_FLAGS) & SHF_IA_64_SHORT
				? T_I_SD : T_I_D);
		case SHF_ALLOC:
			return (T_RO);
		case 0:
			return (T_DEB);
		default:
			return (T_NO);
	}
}
