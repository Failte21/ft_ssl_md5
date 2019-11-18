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
typedef char			*(*t_parse_msg_fn)(char *);

typedef enum	e_command_type
{
	C_STANDARD,
	C_CIPHER,
	C_MDIGEST
}				t_command_type;

typedef struct			s_hash_handler
{
	char			*hash;
	t_hash_fn		hash_fn;
	t_command_type	type;
}						t_hash_handler;

typedef enum	e_type
{
	H_STDIN,
	H_STRING,
	H_FILE
}				t_type;

typedef struct			s_process
{
	t_type				type;
	t_parse_msg_fn		parse_msg_fn;
	char				*input;
	struct s_process	*next;
}						t_process;

typedef struct			s_handler
{
	t_hash_fn	hash_fn;
	char		*hash_name;
	t_process	*processes;
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
	char			flag;
	t_flag_fn		flag_fn;
}						t_flag_handler;

typedef struct			s_parser_handler
{
	t_type			type;
	t_parse_msg_fn	parse_fn;
}						t_parser_handler;

/*
** MD5 Structs
*/

typedef struct		s_md5_tools
{
	uint8_t			*s;
	size_t			s_len;
	uint32_t		aa;
	uint32_t		bb;
	uint32_t		cc;
	uint32_t		dd;
	uint32_t		a;
	uint32_t		b;
	uint32_t		c;
	uint32_t		d;

}					t_md5_tools;

static uint32_t		k_part[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


static uint32_t		s_part[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static uint32_t		m_part[64] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
	5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
	0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
};

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

char					*handle_file(char *filepath);
char					*handle_string(char *s);
char					*handle_stdin(char *s);

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

/*
** Processes
*/
t_process				*push_process(t_process *h, char *input, t_type type);
t_process				*prepend_process(t_process *h, char *i, t_type type);
void					run_processes(t_handler *handler, t_process *head);

void					handle_files(t_handler *handler, char **filespath);

void					display(t_handler *h, t_process *p, char *ha, char *th);

/*
** Usage
*/
void					arg_missing(void);
void					arg_invalid_command(char *command);
void					available_commands(void);

static t_hash_handler	g_hash_table[] =
{
	{ "md5", hash_md5, C_MDIGEST },
	{ "sha256", hash_sha256, C_MDIGEST },
	{ NULL, NULL, 0 },
};

static t_flag_handler	g_flag_handlers[] =
{
	{ 'q', quiet_mode },
	{ 'p', verbose_mode },
	{ 'r', reversed_mode },
	{ 0, NULL }
};

static t_parser_handler	g_parser_handlers[] =
{
	{ H_STRING, handle_string },
	{ H_STDIN, handle_stdin },
	{ H_FILE, handle_file },
	{ 0, NULL }
};

#endif
