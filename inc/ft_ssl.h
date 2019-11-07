#ifndef FT_SSL_H

# define FT_SSL_H

# define BUF_SIZE 1024

# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>

/*
** takes a string and return the hashed string
*/
typedef char			*(*t_hash_fn)(char *);

typedef struct			s_hash_handler
{
	char		*hash;
	t_hash_fn	hash_fn;
}						t_hash_handler;

typedef struct			s_handler
{
	t_hash_fn	hash_fn;
	char		*hash_name;
	char		*flags;
	char		*to_hash;
	char		*filename;
	bool		quiet;
	bool		verbose;
	bool		reversed;

}						t_handler;

typedef struct			s_mem
{
	void	*content;
	size_t	byte_size;
	size_t	n_chunks;
}						t_mem;

/*
** Flag handler
*/
typedef void			(*t_flag_fn)(t_handler *handler);

typedef struct			s_flag_handler
{
	char		flag;
	t_flag_fn	flag_fn;
}						t_flag_handler;

/*
** Flags
*/
void					quiet_mode(t_handler *handler);
void					verbose_mode(t_handler *handler);
void					reversed_mode(t_handler *handler);
t_flag_fn				get_flag_fn(char c);
int						string_mode(t_handler *handler, char *s);

char					*hash_md5(char *s);
char					*hash_sha256(char *s);

char					*get_content(int fd);

int						handle_file(t_handler *handler, char **args);

t_handler				*init_handler(int ac, char **av);
int						handle_flags(t_handler *handler, char **args);
t_hash_fn				get_hash_fn(char *hash);

/*
** Utils
*/
char					*ft_itoa_base_u(unsigned int n, unsigned int base);
char					*ft_itoa_hex_u_fixed(unsigned int n);
uint32_t				to_big_endian (uint32_t num);
uint32_t				rot_right(uint32_t a, size_t b);
uint32_t				**message_to_chunks(uint32_t *content, size_t n_chunks);

void					display(t_handler *handler, char *hashed);

static t_hash_handler	g_hash_table[] =
{
	{ "md5", hash_md5 },
	{ "sha256", hash_sha256 },
	{ NULL, NULL },
};

static t_flag_handler	g_flag_handlers[] =
{
	{ 'q', quiet_mode },
	{ 'p', verbose_mode },
	{ 'r', reversed_mode },
	{ 0, NULL }
};

#endif
