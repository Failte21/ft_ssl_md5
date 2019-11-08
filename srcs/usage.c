#include "../inc/ft_ssl.h"

void		arg_missing(void)
{
	ft_putstr_fd("usage: ft_ssl command [command opts] [command args]", 2);
}

void		arg_invalid_command(char *command)
{
	ft_putstr_fd("Error: ", 2);
	ft_putchar_fd('\'', 2);
	ft_putstr_fd(command, 2);
	ft_putchar_fd('\'', 2);
	ft_putstr_fd(" is an invalid argument\n", 2);
}

static void	print_command_aux(t_command_type type, unsigned int i)
{
	if (g_hash_table[i].hash == NULL)
		return ;
	if (g_hash_table[i].type == type)
	{
		ft_putstr_fd(g_hash_table[i].hash, 2);
		ft_putchar_fd('\n', 2);
	}
	print_command_aux(type, i + 1);
}

static void	print_command(t_command_type type)
{
	print_command_aux(type, 0);
}

void		available_commands(void)
{
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("Standard commands:\n", 2);
	print_command(C_STANDARD);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("Message Digest commands:\n", 2);
	print_command(C_MDIGEST);
	ft_putchar_fd('\n', 2);
	ft_putstr_fd("Cipher commands:\n", 2);
	print_command(C_CIPHER);
}
