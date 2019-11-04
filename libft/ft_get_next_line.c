/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/05 08:05:42 by lsimon            #+#    #+#             */
/*   Updated: 2019/10/07 18:08:40 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	search(char **s, char **tmp, const int fd)
{
	int		r;
	char	buff[BUFF_SIZE + 1];
	char	*str;

	while (*tmp == NULL)
	{
		ft_strclr(buff);
		r = read(fd, buff, BUFF_SIZE);
		if (r < 0)
			return (-1);
		if (r == 0)
		{
			if ((*tmp = ft_strchr(*s, '\0')) == *s)
				return (0);
			return (1);
		}
		buff[r] = '\0';
		if (!(str = ft_strjoin(*s, buff)))
			return (-1);
		ft_strdel(s);
		*s = str;
		*tmp = ft_strchr(*s, '\n');
	}
	return (1);
}

int	get_next_line(const int fd, char **line)
{
	static char	*s = NULL;
	char		*tmp;
	int			rreturn;

	if (fd < 0 || !line)
		return (-1);
	if (!s && !(s = ft_strnew(0)))
		return (-1);
	tmp = ft_strchr(s, '\n');
	rreturn = search(&s, &tmp, fd);
	if (rreturn < 0)
		return (-1);
	if (!(*line = ft_strsub(s, 0, tmp - s)))
		return (-1);
	if (!(tmp = ft_strdup(tmp + 1)))
		return (-1);
	free(s);
	s = tmp;
	return (rreturn);
}
