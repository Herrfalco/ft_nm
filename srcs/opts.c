/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 13:41:07 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 16:20:54 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/opts.h"

static opts_t		g_opts = { 0 };

err_t		opts_init(int argc, char **argv, int *i) {
	uint64_t		j;

	for (*i = 1; *i < argc; ++*i) {
		if (*argv[*i] != '-')
			return (0);
		for (j = 1; argv[*i][j]; ++j) {
			switch (argv[*i][j]) {
				case 'u':
					if (g_opts.u)
						return (E_DUP);
					g_opts.u = 1;
					break;
				case 'r':
					if (g_opts.r)
						return (E_DUP);
					g_opts.r = 1;
					break;
				case 'p':
					if (g_opts.p)
						return (E_DUP);
					g_opts.p = 1;
					break;
				default:
					return (E_OPT);
			}
		}
	}
	return (E_NO);
}

int			opts_get(opt_t op) {
	switch (op) {
		case O_U:
			return (g_opts.u);
		case O_R:
			return (g_opts.r);
		default:
			return (g_opts.p);
	}
}
