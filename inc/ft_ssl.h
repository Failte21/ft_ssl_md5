/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:26:16 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/04 11:44:25 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H

# define FT_SSL_H

# define BUF_SIZE 1024

# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>

/*
** takes a string and return the hashed string
*/
typedef char		*(*t_hash_fn)(char *);

typedef struct		s_hash_handler
{
	char		*hash;
	t_hash_fn	hash_fn;
}					t_hash_handler;

typedef struct		s_handler
{
	t_hash_fn	hash_fn;
	char		*flags;
	char		*to_hash;
}					t_handler;

char				*hash_md5(char *s);
char				*hash_sha256(char *s);

static t_hash_handler	g_hash_table[] =
{
	{ "md5", hash_md5 },
	{ "sha256", hash_sha256 },
	{ NULL, NULL },
};

#endif
