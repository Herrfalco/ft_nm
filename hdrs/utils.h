/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 18:20:03 by fcadet            #+#    #+#             */
/*   Updated: 2023/01/09 19:12:59 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <unistd.h>

#define VAL_BUFF_SZ		32
#define PRINT_BUFF_SZ	1024

typedef struct		buff_s {
	char			dat[PRINT_BUFF_SZ];
	uint64_t		sz;
}					buff_t;

typedef enum		out_e {
	STDOUT = 1,
	STDERR = 2,
}					out_t;

uint64_t	str_len(const char *str);
void		print(out_t out, const char *str);
void		print_char(out_t out, char c);
void		print_n_char(out_t out, char c, uint64_t n);
void		print_flush(out_t out);
int			str_cmp(const char *s1, const char *s2);
int			str_n_cmp(const char *s1, const char *s2, uint64_t n);

#endif // UTILS_H
