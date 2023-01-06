/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 18:40:18 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sym_lst.h"

static sym_lst_t	sym_lst = { 0 };

static const char	type_loc[] = 	"?abCdgiNprstUuVvWw";
static const char	type_glob[] =	"?ABCDGiNpRSTUuVvWw";

void		sym_lst_free(void) {
	free(sym_lst.ents);
}

err_t		sym_lst_init(mem_t *mem) {
	uint64_t		i;
	err_t			err;

	if ((err = sym_init(mem, &sym_lst.ent_nb)))
		return (err);
	if (!(sym_lst.ents = malloc(sizeof(Elf64_Sym *)
			* sym_lst.ent_nb)))
		return (E_ALLOC);
	for (i = 0; i < sym_lst.ent_nb; ++i) {
		if (!(sym_lst.ents[i] = sym_from_idx(i))) {
			sym_lst_free();
			return (E_OOB);
		}
	}
	return (E_NO);
}

err_t		sym_lst_sort(void) {
	uint64_t		i, j, sz;
	char			*s1, *s2;
	Elf64_Sym		*tmp;

	for (sz = sym_lst.ent_nb; sz > 1; --sz) {
		for (i = 0; i + 1 < sz; ++i) {
			if (!(s1 = sym_name(sym_lst.ents[i]))
				|| !(s2 = sym_name(sym_lst.ents[i + 1])))
				return (E_OOB);
			for (j = 0; s1[j] && s1[j] == s2[j]; ++j);
			if (s1[j] > s2[j]) {
				tmp = sym_lst.ents[i];
				sym_lst.ents[i] = sym_lst.ents[i + 1];
				sym_lst.ents[i + 1] = tmp;
			}
		}
	}
	return (E_NO);
}

static void	hex_print(uint64_t val, uint8_t size, uint8_t init) {
	if (init && !val) {
		printf("%*s", size / 4, "");
		return;
	}
	if ((size - 4) / 4)
		hex_print(val / 16, size - 4, 0);
	printf("%c", (char)(val % 16
		+ (val % 16 < 10 ? '0' : 'a' - 10)));
}

err_t		sym_lst_print(void) {
	uint64_t		i;
	Elf64_Sym		*sym;
	err_t			err;
	sym_type_t		type;

	for (i = 0; i < sym_lst.ent_nb; ++i) {
		sym = sym_lst.ents[i];
		if (sym_filt(sym, &err)) {
			if (err)
				return (err);
			continue;
		}
		if (!(type = sym_type(sym)))
			if (!(type = sym_type_from_sec(sym, &err)))
				if (err)
					return (err);
		hex_print(sym->st_value, 64, 1);
		printf(" %c %s\n", ELF64_ST_BIND(sym->st_info)
				== STB_GLOBAL ? type_glob[type]
				: type_loc[type], sym_name(sym));
	}
	return (0);
}
