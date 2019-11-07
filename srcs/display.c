#include "../inc/ft_ssl.h"

static void	print_rev(char *s)
{
	int	i;

	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		write(1, &(s[i]), 1);
		i--;
	}
}

static void	s_to_upper(char *s)
{
	unsigned int i;

	i = 0;
	while (s[i] != '\0')
	{
		s[i] = ft_toupper(s[i]);
		i++;
	}
}

void		display(t_handler *handler, char *hashed)
{
	if (handler->verbose && !handler->quiet)
		ft_putstr(handler->to_hash);
	if (handler->filename != NULL)
	{
		s_to_upper(handler->hash_name);
		ft_putstr(handler->hash_name);
		ft_putstr(" (");
		ft_putstr(handler->filename);
		ft_putstr(") = ");
	}
	if (handler->reversed)
		print_rev(hashed);
	else
		ft_putstr(hashed);
	ft_putchar('\n');
}
