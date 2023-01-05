/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/02 16:23:12 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/05 20:27:18 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list.h"

int			list_init(list_t *list, mem_t *mem, Elf64_Shdr *s_strtab, Elf64_Shdr *s_symtab) {
	uint64_t		i;

	list->mem = mem;
	if (!(list->strs = mem_get(mem, s_strtab->sh_offset,
			0, s_strtab->sh_size))
		|| list->strs[s_strtab->sh_size - 1])
		return (-1);
	if (!s_symtab->sh_entsize)
		return (-1);
	list->sz = s_symtab->sh_size
		/ s_symtab->sh_entsize;
	if (!(list->syms = malloc(sizeof(Elf64_Sym *) * list->sz)))
		return (-1);
	for (i = 0; i < list->sz; ++i) {
		if (!(list->syms[i] = mem_get(mem,
				s_symtab->sh_offset, i,
				s_symtab->sh_entsize))
			|| list->syms[i]->st_name
				> s_strtab->sh_size - 1) {
			free(list->syms);
			return (-1);
		}
	}
	return (0);
}

void		list_free(list_t *list) {
	free(list->syms);
}

void		list_sort(list_t *list) {
	uint64_t		i, j, sz;
	char			*s1, *s2;
	Elf64_Sym		*tmp;

	for (sz = list->sz; sz > 1; --sz) {
		for (i = 0; i + 1 < sz; ++i) {
			s1 = list->strs + list->syms[i]->st_name;
			s2 = list->strs + list->syms[i + 1]->st_name;
			for (j = 0; s1[j] && s1[j] == s2[j]; ++j);
			if (s1[j] > s2[j]) {
				tmp = list->syms[i];
				list->syms[i] = list->syms[i + 1];
				list->syms[i + 1] = tmp;
			}
		}
	}
}

static void	hex_print(uint64_t val, uint8_t size) {
	if ((size - 4) / 4)
		hex_print(val / 16, size - 4);
	printf("%c", (char)(val % 16
		+ (val % 16 < 10 ? '0' : 'a' - 10)));
}

int			list_print(list_t *list) {
	uint64_t		i;
	char			*sec_name, *sym_name;
	uint8_t			glob;

	for (i = 0; i < list->sz; ++i) {
		sym_name = list->strs + list->syms[i]->st_name;
		if (ELF64_ST_TYPE(list->syms[i]->st_info)
				== STT_FILE
			|| !strlen(sym_name)
			|| !strcmp(sym_name, "$d")
			|| !strcmp(sym_name, "$f")
			|| !strcmp(sym_name, "$m")
			|| !strcmp(sym_name, "$p")
			|| !strcmp(sym_name, "$x"))
			continue;

		/*
		printf("%02d %02d %02d ",
				ELF64_ST_TYPE(list->syms[i]->st_info),
				ELF64_ST_BIND(list->syms[i]->st_info),
				ELF64_ST_VISIBILITY(list->syms[i]->st_other));
				*/

		if (!list->syms[i]->st_value)
			printf("                ");
		else
			hex_print(list->syms[i]->st_value, 64);

		// Unsupported:
		// G/g	Small initialized data
		// I	Indirect reference to another symbol
		//
		// N	Debugging (-a)
		// n	Read-only section (R/r duplicated ?) (-a)
		//
		// S/s	Small uninitialized data
		// -	Stab symbol in a.out object
		glob = (ELF64_ST_BIND(list->syms[i]->st_info)
				== STB_GLOBAL);
		if (ELF64_ST_BIND(list->syms[i]->st_info)
				== STB_GNU_UNIQUE)
			printf(" u");
		else if (ELF64_ST_TYPE(list->syms[i]->st_info)
				== STT_GNU_IFUNC)
			printf(" i");
		else if (ELF64_ST_TYPE(list->syms[i]->st_info)
				== SHT_IA_64_UNWIND)
			printf(" p");
		else if (ELF64_ST_BIND(list->syms[i]->st_info)
				== STB_WEAK) {
			if (ELF64_ST_TYPE(list->syms[i]->st_info)
					== STT_OBJECT)
				printf(list->syms[i]->st_value ? " V" : " v");
			else
				printf(list->syms[i]->st_value ? " W" : " w");
		} else if (list->syms[i]->st_shndx >= SHN_LORESERVE) {
			switch (list->syms[i]->st_shndx) {
				case SHN_UNDEF:
					printf(" U");
					break;
				case SHN_ABS:
					printf(" A");
					break;
				case SHN_COMMON:
					printf(" C");
					break;
				default:
					printf(" ?");
			}
		} else if ((sec_name = parse_sname(list->mem,
					list->syms[i]->st_shndx))) {
			if (!strlen(sec_name))
				printf(" U");
			else if (!strcmp(sec_name, ".text")
					|| !strcmp(sec_name, ".plt")
					|| !strcmp(sec_name, ".init")
					|| !strcmp(sec_name, ".fini"))
				printf(glob ? " T" : " t");
			else if (!strcmp(sec_name, ".bss")
					|| !strcmp(sec_name, ".tbss"))
				printf(glob ? " B" : " b");
			else if (!strncmp(sec_name, ".data", 5)
					|| !strcmp(sec_name, ".ctors")
					|| !strcmp(sec_name, ".dynamic")
					|| !strncmp(sec_name, ".got", 4)
					|| !strcmp(sec_name, ".init_array")
					|| !strcmp(sec_name, ".fini_array"))
				printf(glob ? " D" : " d");
			else if (!strncmp(sec_name, ".rodata", 7)
					|| !strncmp(sec_name, ".note", 5)
					|| !strcmp(sec_name, ".gcc_except_table")
					|| !strcmp(sec_name, ".interp")
					|| !strcmp(sec_name, ".dynsym")
					|| !strcmp(sec_name, ".dynstr")
					|| !strcmp(sec_name, ".gnu")
					|| !strcmp(sec_name, ".rela")
					|| !strcmp(sec_name, ".eh_frame")
					|| !strcmp(sec_name, ".eh_frame_hdr"))
				printf(glob ? " R" : " r");
			else if (!strcmp(sec_name, ".comment"))
				printf(glob ? " N" : " n");
			else {
				printf(" %s ", sec_name);
//				return (-1);
			}
		} else
			return (-1);
		printf(" %s\n", list->strs + list->syms[i]->st_name);
	}
	return (0);
}
