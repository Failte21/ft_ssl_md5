/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:20 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:21 by lsimon           ###   ########.fr       */
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
	return (get_flag_fn_aux(c, 0));
}
