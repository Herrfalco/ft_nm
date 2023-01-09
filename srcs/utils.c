/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 18:18:18 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 19:25:00 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/utils.h"

static buff_t	out_buff = { 0 };
static buff_t	err_buff = { 0 };

uint64_t		str_len(const char *str) {
	uint64_t		sz;

	for (sz = 0; *str; ++sz, ++str);
	return (sz);
}

void	print_char(out_t out, char c) {
	switch (out) {
		case STDOUT:
			if (out_buff.sz == PRINT_BUFF_SZ)
				print_flush(out);
			out_buff.dat[out_buff.sz++] = c;
			return;
		default:
			if (err_buff.sz == PRINT_BUFF_SZ)
				print_flush(out);
			err_buff.dat[err_buff.sz++] = c;
	}
}

void	print(out_t out, const char *str) {
	for (; *str; ++str)
		print_char(out, *str);
}

void	print_n_char(out_t out, char c, uint64_t n) {
	for (; n; --n)
		print_char(out, c);
}

void	print_flush(out_t out) {
	switch (out) {
		case STDOUT:
			write(STDOUT, out_buff.dat, out_buff.sz);
			out_buff.sz = 0;
			return;
		default:
			write(STDERR, err_buff.dat, err_buff.sz);
			err_buff.sz = 0;
	}
}

int		str_cmp(const char *s1, const char *s2) {
	for (;; ++s1, ++s2)
		if (!(*s1 && (*s1 == *s2)))
			return (*s2 - *s1);
	return (0);
}

int		str_n_cmp(const char *s1, const char *s2, uint64_t n) {
	for (; n; ++s1, ++s2, --n)
		if (!(*s1 && (*s1 == *s2)))
			return (*s2 - *s1);
	return (0);
}
