#include "../inc/ft_ssl.h"

void				free_content(t_content *content)
{
	if (content != NULL)
	{
		if (content->content != NULL)
			free(content->content);
		free(content);
	}
}

static t_content	*init_content(void *input, size_t len)
{
	t_content	*content;

	content = malloc(sizeof(*content));
	if (content == NULL)
	{
		ft_putstr_fd("Error allocating content struct: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
	}
	content->content = input;
	content->size = len;
	return (content);
}

t_content			*get_content(int fd)
{
	char			buf[BUF_SIZE];
	char			*to_hash;
	char			*tmp;
	int				r;
	unsigned int	len;

	to_hash = NULL;
	len = 0;
	while ((r = read(fd, buf, BUF_SIZE)))
	{
		if (to_hash == NULL)
		{
			to_hash = malloc(len + r);
			ft_memcpy(to_hash, buf, r);
		}
		else
		{
			tmp = to_hash;
			to_hash = malloc(len + r);
			ft_memcpy(to_hash, tmp, len);
			ft_memcpy(to_hash + len, buf, r);
			free(tmp);
		}
		len = len + r;
	}
	to_hash = to_hash != NULL ? to_hash : ft_strdup("");
	return (init_content(to_hash, len));
}

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
	return init_content(ft_strdup(s), ft_strlen(s));
}
