/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 11:54:32 by lsimon            #+#    #+#             */
/*   Updated: 2019/10/10 10:19:37 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s;
	int		i;

	i = 0;
	if (!(s = (char *)malloc(n + 1)))
		return (NULL);
	while (s1[i] && n > 0)
	{
		s[i] = s1[i];
		i++;
		n--;
	}
	s[i] = '\0';
	return (s);
}
