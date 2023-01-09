/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:08 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 11:34:20 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include "mem.h"

typedef enum		err_e {
	E_NO,
	E_FOPEN,
	E_FOPER,
	E_MAP,
	E_EHDR,
	E_SHDR,
	E_SYM,
	E_LIST,
	E_PLIST,
	E_OOB,
	E_ALLOC,
}					err_t;

err_t			error(err_t err, char *file);
err_t			error_close(err_t err, int fd, char *file);
err_t			error_unmap(err_t err, mem_t *mem, char *file);

#endif // ERROR_H
