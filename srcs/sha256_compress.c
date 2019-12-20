/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_compress.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:45:54 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:45:55 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

static void			fill_a(uint32_t a[8], uint32_t h[])
{
	size_t	i;

	i = 0;
	while (i < 8)
	{
		a[i] = h[i];
		i++;
	}
}

static uint32_t		*get_temp(uint32_t a[8], size_t i,
							uint32_t *w, uint32_t k[])
{
	uint32_t	s1;
	uint32_t	ch;
	uint32_t	s0;
	uint32_t	maj;
	uint32_t	*tmp;

	tmp = malloc(2 * sizeof(*tmp));
	s1 = rot_right(a[4], 6) ^ rot_right(a[4], 11) ^ rot_right(a[4], 25);
	ch = (a[4] & a[5]) ^ ((~a[4]) & a[6]);
	tmp[0] = a[7] + s1 + ch + k[i] + w[i];
	s0 = rot_right(a[0], 2) ^ rot_right(a[0], 13) ^ rot_right(a[0], 22);
	maj = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]);
	tmp[1] = s0 + maj;
	return (tmp);
}

static void			update_as(uint32_t as[8], uint32_t temp[2])
{
	as[7] = as[6];
	as[6] = as[5];
	as[5] = as[4];
	as[4] = as[3] + temp[0];
	as[3] = as[2];
	as[2] = as[1];
	as[1] = as[0];
	as[0] = temp[0] + temp[1];
}

static void			update_hs(uint32_t as[8], uint32_t h[])
{
	h[0] = h[0] + as[0];
	h[1] = h[1] + as[1];
	h[2] = h[2] + as[2];
	h[3] = h[3] + as[3];
	h[4] = h[4] + as[4];
	h[5] = h[5] + as[5];
	h[6] = h[6] + as[6];
	h[7] = h[7] + as[7];
}

void				sha256_compress(uint32_t *w, uint32_t h[], uint32_t k[])
{
	size_t		i;
	uint32_t	as[8];
	uint32_t	*temp;

	fill_a(as, h);
	i = 0;
	while (i < 64)
	{
		temp = get_temp(as, i, w, k);
		update_as(as, temp);
		i++;
		free(temp);
	}
	update_hs(as, h);
}
