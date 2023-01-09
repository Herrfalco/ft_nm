/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:47 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 13:31:41 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/error.h"

err_t			error(err_t err, char *file) {
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
		"Out-of-bounds offset",
		"Can't callocate memory",
	};

	fprintf(stderr, "Error: \"%s\": %s\n", file, strs[err]);
	return (err);
}

err_t			error_close(err_t err, int fd, char *file) {
	close(fd);
	return (error(err, file));
}

err_t			error_unmap(err_t err, mem_t *mem, char *file) {
	munmap(mem->data, mem->sz);
	return (error(err, file));
}
