/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:39 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:40 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

int		string_mode(t_handler *handler, char *s)
{
	if (s == NULL)
		return (-1);
	handler->processes = push_process(handler->processes, s, H_STRING);
	return (0);
}
