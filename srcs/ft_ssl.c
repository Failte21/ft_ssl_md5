/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:22:26 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/04 12:51:17 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"


void				quiet_mode(t_handler *handler)
{
	handler->quiet = true;
}

void				verbose_mode(t_handler *handler)
{
	handler->verbose = true;
}

void				reversed_mode(t_handler *handler)
{
	handler->reversed = true;
}

int					string_mode(t_handler *handler, char *s)
{
	if (s == NULL)
		return (-1);
	handler->to_hash = s;
	return (0);
}

static t_flag_fn	get_flag_fn_aux(char c, unsigned int i)
{
	if (g_flag_handlers[i].flag == 0)
	{
		ft_putstr_fd("Unknown flag: ", 1);
		ft_putchar_fd(c, 1);
		ft_putchar_fd('\n', 1);
		return (NULL);
	}
	if (g_flag_handlers[i].flag == c)
		return (g_flag_handlers[i].flag_fn);
	return (get_flag_fn_aux(c, i + 1));
}

t_flag_fn			get_flag_fn(char c)
{
	return get_flag_fn_aux(c, 0);
}

int					handle_flags_aux(t_handler *handler, char **args, unsigned int i)
{
	unsigned int	j;
	t_flag_fn		flag_fn;

	j = 1;
	if (*args == NULL)
		return (0);
	if (*(args[i]) == '-')
	{
		if (ft_strcmp(args[i], "s") == 0)
		{
			i += 1;
			if ((string_mode(handler, args[i + 1])) == -1)
				return (-1);
		}
		else
		{
			while (args[i][j] != '\0')
			{
				if ((flag_fn = get_flag_fn(args[i][j])) == NULL)
					return (-1);
				flag_fn(handler);
			}
		}
	}
	return (handle_flags_aux(handler, args, i + 1));
}

int					handle_flags(t_handler *handler, char **args)
{
	return (handle_flags_aux(handler, args, 0));
}

static t_hash_fn	get_hash_fn_aux(char *hash, unsigned int i)
{
	if (g_hash_table[i].hash == NULL)
		return (NULL);
	if (ft_strcmp(g_hash_table[i].hash, hash) == 0)
		return (g_hash_table[i].hash_fn);
	return (get_hash_fn_aux(hash, i + 1));
}

t_hash_fn			get_hash_fn(char *hash)
{
	t_hash_fn		hash_fn;

	if (hash == NULL)
	{
		printf("(debug) Missing hash argument\n");
		return (NULL);
	}
	if ((hash_fn = (get_hash_fn_aux(hash, 0))) == NULL)
		printf("(debug) Invalid hash argument\n");
	return (hash_fn);
}

char				*get_to_hash()
{
	char			buf[BUF_SIZE];
	char			*to_hash;
	char			*tmp;
	int				r;

	to_hash = NULL;
	while ((r = read(0, buf, BUF_SIZE - 1)))
	{
		buf[r] = '\0';
 		if (to_hash == NULL)
			to_hash = ft_strdup(buf);
		else
		{
			tmp = to_hash;
			to_hash = malloc(ft_strlen(tmp) + ft_strlen(buf) + 1);
			ft_strcpy(to_hash, tmp);
			ft_strcat(to_hash, buf);
			free(tmp);
		}
	}
	return (to_hash);
}


t_handler			*init_handler(int ac, char **av)
{
	t_handler		*handler;
	t_hash_fn		hash_fn;
	char			*to_hash;
	int				i;

	i = 1;
	while (i < ac)
	{
		printf("(debug) av[i]: %s\n", av[i]);
		i++;
	}
	if ((hash_fn = get_hash_fn(av[1])) == NULL)
		return (NULL);
	if ((to_hash = get_to_hash()) == NULL)
		return (NULL);
	printf("(debug) to_hash: %s\n", to_hash);
	handler = malloc(sizeof(t_handler));
	if (handler == NULL)
	{
		ft_putstr_fd("Error allocating memory for handler structure", 1);
		return (NULL);
	}
	handler->hash_fn = hash_fn;
	handler->flags = NULL;
	handler->to_hash = to_hash;
	return (handler);
}

int					main(int ac, char **av)
{
	t_handler	*handler;
	char		*hashed;

	handler = init_handler(ac, av);
	if (handler == NULL)
		return (1);
	if ((hashed = handler->hash_fn(handler->to_hash)) == NULL)
		return (1);
	printf("(debug) hashed: %s\n", hashed);
	return (0);
}
