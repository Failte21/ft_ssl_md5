/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:22:26 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/07 17:12:12 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

int					main(int ac, char **av)
{
	t_handler	*h;

	h = init_handler(ac, av);
	if (h == NULL)
		return (1);
	if (handle_flags(h, av + 2) == -1)
	{
		free(h);
		return (-1);
	}
	if (ac == 2 || h->processes == NULL || h->verbose)
		h->processes = push_process(h->processes, NULL, H_STDIN);
	if (h->processes == NULL)
	{
		free(h);
		return (1);
	}
	run_processes(h, h->processes);
	return (0);
}
