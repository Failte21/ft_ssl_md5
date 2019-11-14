#include "../inc/ft_ssl.h"

t_content			*handle_file(char *filepath)
{
	int			fd;
	t_content	*content;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("ft_ssl: ", 2);
		ft_putstr_fd(filepath, 2);
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
		return (NULL);
	}
	content = get_content(fd);
	close(fd);
	return (content);
}

t_content			*handle_stdin(char *s)
{
	(void)s;
	return (get_content(0));
}

t_content			*handle_string(char *s)
{
	return (init_content(ft_strdup(s), ft_strlen(s)));
}

t_parse_msg_fn		get_msg_fn(t_type type, unsigned int i)
{
	if (g_parser_handlers[i].type == type)
		return (g_parser_handlers[i].parse_fn);
	return (get_msg_fn(type, i + 1));
}
