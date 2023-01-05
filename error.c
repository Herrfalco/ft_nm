/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:47 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/05 16:34:20 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

int				error(t_err err, char *file) {
	const char		*strs[] = {
		"No error",
		"Can't open file",
		"Can't access file",
		"Unsupported file",
		"Can't parse program header",
		"Can't parse desired section",
		"No symbol found",
		"Can't initialize symbol list",
		"Can't print symbol list",
	};

	fprintf(stderr, "Error: \"%s\": %s\n", file, strs[err]);
	return (err);
}

int				error_close(t_err err, int fd, char *file) {
	close(fd);
	return (error(err, file));
}

int				error_unmap(t_err err, mem_t *mem, char *file) {
	munmap(mem->data, mem->sz);
	return (error(err, file));
}
