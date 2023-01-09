/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 09:56:38 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 13:31:54 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/mem.h"

void		*mem_get(mem_t *mem, uint64_t offset, uint64_t idx, uint64_t size) {
	if (!size || offset + (idx + 1) * size > mem->sz)
		return (NULL);
	return (mem->data + offset + idx * size);
}
