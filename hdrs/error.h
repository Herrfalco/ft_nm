/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:08 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/11 09:28:33 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
#define ERROR_H

#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>
#include "mem.h"
#include "utils.h"

typedef enum		err_e {
	E_NO,
	E_OPT,
	E_DUP,
	E_FOPEN,
	E_FILE,
	E_FINFO,
	E_EHDR,
	E_SYM,
	E_OOB,
	E_ALLOC,
	E_ARCH,
	E_SYMNT,
	E_SECNT,
	E_SYMSZ,
}					err_t;

err_t			error(err_t err, char *file);
err_t			error_close(err_t err, int fd, char *file);
err_t			error_unmap(err_t err, mem_t *mem, char *file);

#endif // ERROR_H
