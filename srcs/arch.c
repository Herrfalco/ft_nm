/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 22:17:22 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 13:31:35 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/arch.h"

static uint8_t		g_elf_class = 0;

int				arch_is_64(void) {
	return (g_elf_class == ELFCLASS64);
}

int				arch_set(uint8_t elf_class) {
	return ((g_elf_class = elf_class) != ELFCLASS32
			&& !arch_is_64());
}

uint64_t		arch_macro(void *arg, macro_t mac) {
	switch (mac) {
		case M_ST_TYPE:
			return (arch_is_64()
				? ELF64_ST_TYPE(*((uint8_t *)arg))
				: ELF32_ST_TYPE(*((uint8_t *)arg)));
		case M_ST_BIND:
			return (arch_is_64()
				? ELF64_ST_BIND(*((uint8_t *)arg))
				: ELF32_ST_BIND(*((uint8_t *)arg)));
		default:
			return (0);
	}
}

uint64_t		arch_ehdr(void *e_hdr, ehdr_fld_t fld) {
	Elf64_Ehdr		*e_hdr_64 = e_hdr;
	Elf32_Ehdr		*e_hdr_32 = e_hdr;

	switch (fld) {
		case EF_SHSTRNDX:
			return (arch_is_64() ? e_hdr_64->e_shstrndx
				: e_hdr_32->e_shstrndx);
		case EF_SHOFF:
			return (arch_is_64() ? e_hdr_64->e_shoff
				: e_hdr_32->e_shoff);
		case EF_SHENTSIZE:
			return (arch_is_64() ? e_hdr_64->e_shentsize
				: e_hdr_32->e_shentsize);
		case EF_SHNUM:
			return (arch_is_64() ? e_hdr_64->e_shnum
				: e_hdr_32->e_shnum);
		default:
			return (0);
	}
}

uint64_t	arch_shdr(void *s_hdr, shdr_fld_t fld) {
	Elf64_Shdr		*s_hdr_64 = s_hdr;
	Elf32_Shdr		*s_hdr_32 = s_hdr;

	switch (fld) {
		case SF_NAME:
			return (arch_is_64() ? s_hdr_64->sh_name
				: s_hdr_32->sh_name);
		case SF_LINK:
			return (arch_is_64() ? s_hdr_64->sh_link
				: s_hdr_32->sh_link);
		case SF_OFFSET:
			return (arch_is_64() ? s_hdr_64->sh_offset
				: s_hdr_32->sh_offset);
		case SF_SIZE:
			return (arch_is_64() ? s_hdr_64->sh_size
				: s_hdr_32->sh_size);
		case SF_ENTSIZE:
			return (arch_is_64() ? s_hdr_64->sh_entsize
				: s_hdr_32->sh_entsize);
		case SF_TYPE:
			return (arch_is_64() ? s_hdr_64->sh_type
				: s_hdr_32->sh_type);
		case SF_FLAGS:
			return (arch_is_64() ? s_hdr_64->sh_flags
				: s_hdr_32->sh_flags);
		default:
			return (0);
	}
}

uint64_t	arch_sym(void *sym, sym_fld_t fld) {
	Elf64_Sym		*sym_64 = sym;
	Elf32_Sym		*sym_32 = sym;

	switch (fld) { 
		case SMF_NAME:
			return (arch_is_64() ? sym_64->st_name
				: sym_32->st_name);
		case SMF_INFO:
			return (arch_is_64() ? sym_64->st_info
				: sym_32->st_info);
		case SMF_SHNDX:
			return (arch_is_64() ? sym_64->st_shndx
				: sym_32->st_shndx);
		case SMF_VALUE:
			return (arch_is_64() ? sym_64->st_value
				: sym_32->st_value);
		default:
			return (0);
	}
}
