/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opts.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 13:38:36 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 16:20:03 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPTS_H
#define OPTS_H

#include <stdint.h>
#include "error.h"

typedef enum		opt_e {
	O_U,
	O_R,
	O_P,
}					opt_t;

typedef struct		opts_s {
	uint8_t			u : 1;
	uint8_t			r : 1;
	uint8_t			p : 1;
}					opts_t;

err_t				opts_init(int argc, char **argv, int *i);
int					opts_get(opt_t op);

#endif // OPTS_H
