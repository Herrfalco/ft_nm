/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:17:07 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 17:58:32 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
#define PARSE_H

#include <elf.h>
#include <string.h>
#include "mem.h"

Elf64_Ehdr			*parse_ehdr(mem_t *mem);
Elf64_Shdr			*parse_shdr(mem_t *mem, const char *name);

#endif // PARSE_H
