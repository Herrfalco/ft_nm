/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:08 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/05 16:23:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include "mem.h"

typedef enum		e_err {
	E_NO,
	E_FOPEN,
	E_FOPER,
	E_ALLOC,
	E_EHDR,
	E_SHDR,
	E_SYM,
	E_LIST,
	E_PLIST,
}					t_err;

int				error(t_err err, char *file);
int				error_close(t_err err, int fd, char *file);
int				error_unmap(t_err err, mem_t *mem, char *file);

#endif // ERROR_H
