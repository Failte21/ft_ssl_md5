/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:18 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:19 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

int					main(int ac, char **av)
{
	t_handler	*h;

	h = init_handler(av);
	if (h == NULL)
	{
		available_commands();
		return (1);
	}
	if (handle_flags(h, av + 2) == -1)
	{
		free(h);
		return (-1);
	}
	if (ac == 2 || h->processes == NULL || h->verbose)
		h->processes = prepend_process(h->processes, NULL, H_STDIN);
	if (h->processes == NULL)
	{
		free(h);
		return (1);
	}
	run_processes(h, h->processes);
	free_handler(h);
	return (0);
}
