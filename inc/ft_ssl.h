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
