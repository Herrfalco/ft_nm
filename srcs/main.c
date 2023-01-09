/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:21:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 15:36:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/main.h"

uint8_t		g_elf_class = 0;

static err_t			treat_file(char *file, uint8_t multi) {
	int					fd = open(file, O_RDONLY);
	mem_t				mem;
	struct stat			statbuf;
	err_t				err;

	if (fd < 0)
		return (error(E_FOPEN, file));
	if (fstat(fd, &statbuf))
		return (error_close(E_FOPER, fd, file));
	mem.sz = statbuf.st_size;
	if ((mem.data = mmap(NULL, mem.sz, PROT_READ,
			MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (error_close(E_ALLOC, fd, file));
	close(fd);
	if ((err = sym_lst_init(&mem))
		|| (err = sym_lst_sort()))
		return (error_unmap(err, &mem, file));
	if (multi)
		printf("\n%s:\n", file);
	if ((err = sym_lst_print()))
		return (error_unmap(err, &mem, file));
	sym_lst_free();
	munmap(mem.data, statbuf.st_size);
	return (E_NO);
}

int			main(int argc, char **argv) {
	int		i, ret = 0, rem;
	err_t	err;

	if ((err = opts_init(argc, argv, &i)))
		return (error(err, argv[i]));
	if (!(rem = argc - i))
		return (treat_file("a.out", 0));
	for (; i < argc; ++i)
		ret |= treat_file(argv[i], rem > 1);
	return (ret);
}
