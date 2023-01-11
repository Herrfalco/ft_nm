/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/03 10:03:47 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/11 09:29:26 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/error.h"

err_t			error(err_t err, char *file) {
	const char		*strs[] = {
		"No error",
		"Unknown option",
		"Duplicated option",
		"Can't open file",
		"Not a regular file",
		"Can't get file informations",
		"Unsupported file",
		"No symbol found",
		"Out-of-bounds offset",
		"Can't allocate memory",
		"Unsupported architechture",
		"Invalid symbol names table",
		"Invalid section names table",
		"Invalid symbol table entry size",
	};

	print(STDERR, "Error: \"");
	print(STDERR, file);
	print(STDERR, "\": ");
	print(STDERR, strs[err]);
	print_char(STDERR, '\n');
	print_flush(STDERR);
	return (err);
}

err_t			error_close(err_t err, int fd, char *file) {
	close(fd);
	return (error(err, file));
}

void		sym_lst_free(void);

err_t			error_unmap(err_t err, mem_t *mem, char *file) {
	munmap(mem->data, mem->sz);
	sym_lst_free();
	return (error(err, file));
}
