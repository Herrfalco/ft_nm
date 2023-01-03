/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:47 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/03 11:17:30 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

int				error(t_err err) {
	const char		*strs[] = {
		"Invalid arguent",
		"Can't open file",
		"File operation failed",
		"Can't allocate memory",
		"Can't parse program header",
		"Can't parse desired section",
		"No symbol found",
	};

	fprintf(stderr, "Error: %s\n", strs[err]);
	return (err + 1);
}

int				error_close(t_err err, int fd) {
	close(fd);
	return (error(err));
}

int				error_unmap(t_err err, void *mem, uint64_t mem_sz) {
	munmap(mem, mem_sz);
	return (error(err));
}
