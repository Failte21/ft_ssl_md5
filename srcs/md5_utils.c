/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:08 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:09 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

uint32_t	left_rotate(uint32_t elem1, uint32_t ele)
{
	return ((elem1 << ele) | (elem1 >> (32 - ele)));
}

void		add_save_values(t_md5_tools *md5)
{
	md5->aa = md5->a + md5->aa;
	md5->bb = md5->b + md5->bb;
	md5->cc = md5->c + md5->cc;
	md5->dd = md5->d + md5->dd;
}

void		init(t_md5_tools *md5)
{
	md5->a = md5->aa;
	md5->b = md5->bb;
	md5->c = md5->cc;
	md5->d = md5->dd;
}

uint32_t	ft_bit_swapping(uint32_t swap)
{
	uint32_t	swap_bit;

	swap_bit = ((swap >> 24) |
		((swap >> 8) & 0xff00) |
		((swap << 8) & 0xff0000) |
		(swap << 24));
	return (swap_bit);
}

void		fix_endian(t_md5_tools *md5)
{
	md5->aa = ft_bit_swapping(md5->aa);
	md5->bb = ft_bit_swapping(md5->bb);
	md5->cc = ft_bit_swapping(md5->cc);
	md5->dd = ft_bit_swapping(md5->dd);
}
