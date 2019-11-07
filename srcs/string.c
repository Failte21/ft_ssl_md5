/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:20:36 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/07 16:32:05 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

char					*handle_string(char *s)
{
	return (s);
}

int					string_mode(t_handler *handler, char *s)
{
	if (s == NULL)
		return (-1);
	handler->processes = push_process(handler->processes, s, H_STRING);
	return (0);
}
