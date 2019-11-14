#ifndef FT_SSL_H

# define FT_SSL_H

# define BUF_SIZE 1024

# include "../libft/libft.h"
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>

typedef struct			s_content
{
	size_t	size;
	void	*content;
}						t_content;

typedef char			*(*t_hash_fn)(t_content *);
typedef t_content		*(*t_parse_msg_fn)(char *);

typedef enum			e_command_type
{
	C_STANDARD,
	C_CIPHER,
	C_MDIGEST
}						t_command_type;

typedef struct			s_hash_handler
{
	char			*hash;
	t_hash_fn		hash_fn;
	t_command_type	type;
}						t_hash_handler;

typedef enum			e_type
{
	H_STDIN,
	H_STRING,
	H_FILE
}						t_type;

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
** Flags
*/
void					quiet_mode(t_handler *handler);
void					verbose_mode(t_handler *handler);
void					reversed_mode(t_handler *handler);
t_flag_fn				get_flag_fn(char c);
int						string_mode(t_handler *handler, char *s);

char					*hash_md5(t_content *content);
char					*hash_sha256(t_content *content);

/*
** Content
*/

t_content				*init_content(void *input, size_t len);
t_content				*get_content(int fd);
void					free_content(t_content *content);

/*
** Input
*/

t_content				*handle_file(char *filepath);
t_content				*handle_string(char *s);
t_content				*handle_stdin(char *s);
t_parse_msg_fn			get_msg_fn(t_type type, unsigned int i);

t_handler				*init_handler(char **av);
void					free_handler(t_handler *handler);
int						handle_flags(t_handler *handler, char **args);
t_hash_fn				get_hash_fn(char *hash);

/*
** Utils
*/
char					*ft_itoa_base_u(unsigned int n, unsigned int base);
char					*ft_itoa_hex_u_fixed(unsigned int n);
uint32_t				to_big_endian (uint32_t num);
uint32_t				rot_right(uint32_t a, size_t b);

/*
** Processes
*/
t_process				*push_process(t_process *h, char *s, t_type t);
t_process				*prepend_process(t_process *h, char *s, t_type t);
void					run_processes(t_handler *handler, t_process *head);
void					free_processes(t_process *head);

void					handle_files(t_handler *handler, char **filespath);

void					display(t_handler *h, t_process *p,
								char *s, t_content c);

/*
** Usage
*/
void					arg_missing(void);
void					arg_invalid_command(char *command);
void					available_commands(void);

/*
** Sha256
*/

void					sha256_compress(uint32_t *w, uint32_t h[],
							uint32_t k[]);
t_mem					sha256_pad(t_content *c);

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
