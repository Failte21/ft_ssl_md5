#include "../inc/ft_ssl.h"

char	*handle_string(char *s)
{
	return (s);
}

int		string_mode(t_handler *handler, char *s)
{
	if (s == NULL)
		return (-1);
	handler->processes = push_process(handler->processes, s, H_STRING);
	return (0);
}
