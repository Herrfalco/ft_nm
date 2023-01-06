/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 11:47:02 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 18:43:03 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sym.h"

static sym_dat_t		sym_dat = { 0 };

err_t			sym_init(mem_t *mem, uint64_t *sym_nb) {
	Elf64_Shdr		*s_strtab, *s_symtab;
	err_t			err;

	if ((err = sec_init(mem)))
		return (err);
	sym_dat.mem = mem;
	if (!(s_strtab = sec_from_name(".strtab"))
		|| !(s_symtab = sec_from_name(".symtab")))
		return (E_SYM);
	sym_dat.offset = s_symtab->sh_offset;
	if (!(sym_dat.ent_sz = s_symtab->sh_entsize))
		return (E_SYM);
	sym_dat.ent_nb = s_symtab->sh_size / sym_dat.ent_sz;
	*sym_nb = sym_dat.ent_nb;
	sym_dat.names_sz = s_strtab->sh_size;
	if (!(sym_dat.names = mem_get(mem, s_strtab->sh_offset,
			0, sym_dat.names_sz))
		|| sym_dat.names[sym_dat.names_sz - 1])
		return (E_SYM);
	return (E_NO);
}

Elf64_Sym		*sym_from_idx(uint64_t idx) {
	if (idx >= sym_dat.ent_nb)
		return (NULL);
	return (mem_get(sym_dat.mem, sym_dat.offset,
		idx, sym_dat.ent_sz));
}

char			*sym_name(Elf64_Sym *sym) {
	if (sym->st_name > sym_dat.names_sz - 1)
		return (NULL);
	return (sym_dat.names + sym->st_name);
}

int				sym_filt(Elf64_Sym *sym, err_t *err) {
	uint8_t		type = ELF64_ST_TYPE(sym->st_info);
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

sym_type_t		sym_type(Elf64_Sym *sym) {
	uint8_t		type = ELF64_ST_TYPE(sym->st_info),
				bind = ELF64_ST_BIND(sym->st_info);

	if (bind == STB_GNU_UNIQUE)
		return (T_UNIQ);
	else if (type == STT_GNU_IFUNC)
		return (T_REL);
	else if (bind == STB_WEAK)
		return (type == STT_OBJECT
			? (sym->st_shndx == SHN_UNDEF
				? T_WK_O_UD : T_WK_O)
			: (sym->st_shndx == SHN_UNDEF
				? T_WK_UD : T_WK));
	else if (sym->st_shndx == SHN_UNDEF)
		return (T_UD);
	else if (sym->st_shndx == SHN_ABS)
		return (T_ABS);
	else if (sym->st_shndx == SHN_COMMON)
		return (T_COM);
	return (T_NO);
}

sym_type_t		sym_type_from_sec(Elf64_Sym *sym, err_t *err) {
	Elf64_Shdr		*sec;
	uint64_t		sec_prof;

	if ((*err = (sec = sec_from_idx(sym->st_shndx))
		? E_NO : E_OOB))
		return (T_NO);
	if (sec->sh_type == SHT_NOBITS)
		return (sec->sh_flags & SHF_IA_64_SHORT
			? T_UI_SD : T_UI_D);
	else if (sec->sh_type == SHT_IA_64_UNWIND)
		return (T_UNW);
	sec_prof = sec->sh_flags
		& (SHF_ALLOC | SHF_WRITE | SHF_EXECINSTR);
	switch (sec_prof) {
		case SHF_ALLOC | SHF_EXECINSTR:
			return (T_TXT);
		case SHF_ALLOC | SHF_WRITE:
			return (sec->sh_flags & SHF_IA_64_SHORT ?
				T_I_SD : T_I_D);
		case SHF_ALLOC:
			return (T_RO);
		case 0:
			return (T_DEB);
		default:
			return (T_NO);
	}
}
