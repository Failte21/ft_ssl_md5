/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:08:49 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/04 16:26:49 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

static int			apply_flags(t_handler *handler, char *flags)
{
	unsigned int	j;
	t_flag_fn		flag_fn;

	j = 1;
	while (flags[j] != '\0')
	{
		if ((flag_fn = get_flag_fn(flags[j])) == NULL)
			return (-1);
		flag_fn(handler);
		j++;
	}
	return (0);
}

static int			handle_flags_aux(t_handler *h, char **args, unsigned int i)
{
	if (args[i] == NULL)
		return (0);
	if (*(args[i]) == '-')
	{
		if (ft_strcmp(args[i], "-s") == 0)
		{
			i += 1;
			if ((string_mode(h, args[i])) == -1)
				return (-1);
		}
		else
		{
			if (apply_flags(h, args[i]) == -1)
				return (-1);
		}
		return (handle_flags_aux(h, args, i + 1));
	}
	return (handle_file(h, args + i));
}

int					handle_flags(t_handler *handler, char **args)
{
	return (handle_flags_aux(handler, args, 0));
}

t_handler			*init_handler(int ac, char **av)
{
	t_handler		*handler;
	t_hash_fn		hash_fn;

	(void)ac;
	if ((hash_fn = get_hash_fn(av[1])) == NULL)
		return (NULL);
	handler = malloc(sizeof(t_handler));
	if (handler == NULL)
	{
		ft_putstr_fd("Error allocating memory for handler structure", 1);
		return (NULL);
	}
	handler->hash_fn = hash_fn;
	handler->flags = NULL;
	handler->to_hash = NULL;
	handler->reversed = false;
	handler->quiet = false;
	handler->verbose = false;
	return (handler);
}
