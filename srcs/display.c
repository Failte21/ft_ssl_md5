#include "../inc/ft_ssl.h"

// static void	print_rev(char *s)
// {
// 	int	i;

// 	i = ft_strlen(s) - 1;
// 	while (i >= 0)
// 	{
// 		write(1, &(s[i]), 1);
// 		i--;
// 	}
// }

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

static void	display_filename(t_handler *handler, char *filename)
{
	if (!handler->reversed)
	{
		s_to_upper(handler->hash_name);
		ft_putstr(handler->hash_name);
		ft_putstr(" (");
	}
	else
		ft_putchar(' ');
	ft_putstr(filename);
	if (!handler->reversed)
		ft_putstr(") = ");
}

void		display(t_handler *handler, t_process *process, char *hashed)
{
	if (handler->verbose)
		ft_putstr(process->input);
	if (process->type == H_FILE && !handler->reversed)
		display_filename(handler, process->input);
	ft_putstr(hashed);
	if (process->type == H_FILE && handler->reversed)
		display_filename(handler, process->input);
	ft_putchar('\n');
}
