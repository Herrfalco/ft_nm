/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sym_lst.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 15:33:17 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/sym_lst.h"

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
	if (!(sym_lst.ents = malloc((arch_is_64()
			? sizeof(Elf64_Sym *) : sizeof(Elf32_Sym *))
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
	void			*tmp;

	for (sz = sym_lst.ent_nb; sz > 1; --sz) {
		for (i = 0; i + 1 < sz; ++i) {
			if (!(s1 = sym_name(sym_lst.ents[i]))
				|| !(s2 = sym_name(sym_lst.ents[i + 1])))
				return (E_OOB);
			for (j = 0; s1[j] && s1[j] == s2[j]; ++j);
			if (opts_get(O_R) ? s1[j] < s2[j] : s1[j] > s2[j]) {
				tmp = sym_lst.ents[i];
				sym_lst.ents[i] = sym_lst.ents[i + 1];
				sym_lst.ents[i + 1] = tmp;
			}
		}
	}
	return (E_NO);
}

static void	hex_print(uint64_t val, uint8_t size) {
	if ((size - 4) / 4)
		hex_print(val / 16, size - 4);
	printf("%c", (char)(val % 16
		+ (val % 16 < 10 ? '0' : 'a' - 10)));
}

err_t		sym_lst_print(void) {
	uint64_t		i;
	uint8_t			info;
	void			*sym;
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
		if (type == T_UD || type == T_WK_UD)
			printf("%*s", arch_is_64() ? 16 : 8, "");
		else if (opts_get(O_U))
			continue;
		else
			hex_print(arch_sym(sym, SMF_VALUE), 
				arch_is_64() ? 64 : 32);
		info = arch_sym(sym, SMF_INFO);
		printf(" %c %s\n", arch_macro(&info, M_ST_BIND)
				== STB_GLOBAL ? type_glob[type]
				: type_loc[type], sym_name(sym));
	}
	return (0);
}
