#include "../inc/ft_ssl.h"

static int	get_file_content(t_handler *handler, char *filepath)
{
	int		fd;
	char	*to_hash;

	fd = open(filepath, O_RDONLY);
	handler->filename = filepath;
	if (fd == -1)
	{
		printf("(debug): open error %s\n", strerror(errno));
		return (-1);
	}
	if ((to_hash = get_content(fd)) == NULL)
	{
		printf("(debug) error while reading the file\n");
		return (-1);
	}
	handler->to_hash = to_hash;
	return (0);
}

int			handle_file(t_handler *handler, char **args)
{
	if (*args != NULL)
	{
		if (handler->to_hash != NULL)
		{
			printf("(debug) conflict s flag and file\n");
			return (-1);
		}
		if (*(args + 1) != NULL)
		{
			printf("(debug) Error too many args\n");
			return (-1);
		}
		return (get_file_content(handler, *args));
	}
	return (0);
}
