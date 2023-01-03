/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 09:49:16 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 11:05:30 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <unistd.h>

typedef struct		mem_s {
	void			*data;
	uint64_t		sz;
}					mem_t;

void		*mem_get(mem_t *mem, uint64_t offset, uint64_t idx, uint64_t size);

#endif
