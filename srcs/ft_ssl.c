/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:22:26 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/07 13:11:29 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

int					main(int ac, char **av)
{
	t_handler	*h;
	char		*hashed;

	h = init_handler(ac, av);
	if (h == NULL)
		return (1);
	if (handle_flags(h, av + 2) == -1)
	{
		free(h);
		return (-1);
	}
	h->to_hash = h->to_hash == NULL ? get_content(0) : h->to_hash;
	if (h->to_hash == NULL)
	{
		free(h);
		return (1);
	}
	if ((hashed = h->hash_fn(h->to_hash)) == NULL)
	{
		free(h);
		return (1);
	}
	display(h, hashed);
	return (0);
}
