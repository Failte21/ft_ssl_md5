/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   content.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 16:04:49 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/07 11:24:18 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

char				*get_content(int fd)
{
	char			buf[BUF_SIZE];
	char			*to_hash;
	char			*tmp;
	int				r;
	unsigned int	len;

	to_hash = NULL;
	while ((r = read(fd, buf, BUF_SIZE - 1)))
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
	len = ft_strlen(to_hash) - 1;
	return (to_hash);
}
