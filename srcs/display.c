/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:13:13 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/04 16:13:46 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

static void	print_rev(char *s)
{
	int	i;

	i = ft_strlen(s) - 1;
	while (i >= 0)
	{
		write(0, &(s[i]), 1);
		i--;
	}
}

void		display(t_handler *handler, char *hashed)
{
	if (handler->verbose && !handler->quiet)
	{
		ft_putstr(handler->to_hash);
		ft_putchar('\n');
	}
	if (handler->reversed)
		print_rev(hashed);
	else
		ft_putstr(hashed);
	ft_putchar('\n');
}
