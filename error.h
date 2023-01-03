/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:08 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 11:17:40 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

typedef enum		e_err {
	E_ARG,
	E_FOPEN,
	E_FOPER,
	E_ALLOC,
	E_EHDR,
	E_SHDR,
	E_SYM,
}					t_err;

int				error(t_err err);
int				error_close(t_err err, int fd);
int				error_unmap(t_err err, void *mem, uint64_t mem_sz);

#endif // ERROR_H
