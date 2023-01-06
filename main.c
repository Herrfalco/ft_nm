/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 15:21:00 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/06 18:15:58 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <elf.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include "sym_lst.h"
#include "mem.h"
#include "error.h"
#include "sec.h"

static int			treat_file(char *file, uint8_t multi) {
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
	int		i, ret = 0;

	if (argc == 1)
		return (treat_file("a.out", 0));
	for (i = 1; i < argc; ++i)
		ret |= treat_file(argv[i], argc > 2);
	return (ret);
}
