#include "../inc/ft_ssl.h"

char			*handle_file(char *filepath)
{
	int		fd;
	char	*to_hash;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("ft_ssl: ", 1);
		ft_putstr_fd(filepath, 1);
		ft_putstr_fd(": ", 1);
		ft_putstr_fd(strerror(errno), 1);
		ft_putchar_fd('\n', 1);
		return (NULL);
	}
	if ((to_hash = get_content(fd)) == NULL)
		return (NULL);
	return (to_hash);
}

void			handle_files_aux(t_handler *h, char **filespath, unsigned int i)
{
	if (filespath[i] == NULL)
		return ;
	h->processes = push_process(h->processes, filespath[i], H_FILE);
	handle_files_aux(h, filespath, i + 1);
}

void			handle_files(t_handler *handler, char **filespath)
{
	handle_files_aux(handler, filespath, 0);
}
