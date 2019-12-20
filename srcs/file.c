/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:21 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:22 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

void			handle_files_aux(t_handler *h, char **filespath, unsigned int i)
{
	if (filespath[i] == NULL)
		return ;
	h->processes = push_process(h->processes, filespath[i], H_FILE);
	handle_files_aux(h, filespath, i + 1);
}

void			handle_files(t_handler *handler, char **filespath)
{
	handle_files_aux(handler, filespath, 0);
}
