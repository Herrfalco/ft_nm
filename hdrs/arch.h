/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arch.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 22:19:21 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 12:06:10 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARCH_H
#define ARCH_H

#include <stdint.h>
#include <elf.h>

typedef enum		ehdr_fld_e {
	EF_SHSTRNDX,
	EF_SHOFF,
	EF_SHENTSIZE,
	EF_SHNUM,
}					ehdr_fld_t;

typedef enum		shdr_fld_e {
	SF_NAME,
	SF_LINK,
	SF_OFFSET,
	SF_SIZE,
	SF_ENTSIZE,
	SF_TYPE,
	SF_FLAGS,
}					shdr_fld_t;

typedef enum		sym_fld_e {
	SMF_NAME,
	SMF_INFO,
	SMF_SHNDX,
	SMF_VALUE,
}					sym_fld_t;

typedef enum		macro_e {
	M_ST_TYPE,
	M_ST_BIND,
}					macro_t;

int			arch_is_64(void);
int			arch_set(uint8_t elf_class);
uint64_t	arch_macro(void *arg, macro_t mac);
uint64_t	arch_ehdr(void *e_hdr, ehdr_fld_t fld);
uint64_t	arch_shdr(void *s_hdr, shdr_fld_t fld);
uint64_t	arch_sym(void *sym, sym_fld_t fld);

#endif // ARCH_H
