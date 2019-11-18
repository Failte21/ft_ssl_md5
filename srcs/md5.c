/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:54:27 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/04 16:16:21 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"
#include <stdio.h>

typedef struct		s_md5
{
	uint32_t		a0;
	uint32_t		b0;
	uint32_t		c0;
	uint32_t		d0;
	uint32_t		a;
	uint32_t		b;
	uint32_t		c;
	uint32_t		d;
	size_t			msg_len;
	uint8_t			*msg;
}					t_md5;

static uint32_t		g_md5_k[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

static uint32_t		g_md5_s[64] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static uint32_t		g_md5_m[64] =
{
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
	1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
	5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
	0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
};

uint32_t		md5_left_rotate(uint32_t f, uint32_t s)
{
	return ((f << s) | (f >> (32 - s)));
}

void			md5_addback(t_md5 *md5)
{
	md5->a0 = md5->a + md5->a0;
	md5->b0 = md5->b + md5->b0;
	md5->c0 = md5->c + md5->c0;
	md5->d0 = md5->d + md5->d0;
}

void			md5_addstart(t_md5 *md5)
{
	md5->a = md5->a0;
	md5->b = md5->b0;
	md5->c = md5->c0;
	md5->d = md5->d0;
}

uint32_t		md5_swap_bit(uint32_t swap)
{
	uint32_t	r;

	r = ((swap >> 24) |
			((swap >> 8) & 0xff00) |
			((swap << 8) & 0xff0000) |
			(swap << 24));
	return (r);
}

void			md5_endianfix(t_md5 *md5)
{
	md5->a0 = md5_swap_bit(md5->a0);
	md5->b0 = md5_swap_bit(md5->b0);
	md5->c0 = md5_swap_bit(md5->c0);
	md5->d0 = md5_swap_bit(md5->d0);
}

static void		md5_print(t_md5 *md5)
{
	md5_endianfix(md5);
	printf("ALORS VOILA : %08x%08x%08x%08x\n", md5->a0, md5->b0, md5->c0, md5->d0);
	return ;
}

static void		md5_arg(uint32_t *msg, t_md5 *md5)
{
	uint32_t	i;
	uint32_t	f;
	uint32_t	temp;

	i = 0;
	md5_addstart(md5);
	while (i < 64)
	{
		if (i < 16)
			f = (md5->b & md5->c) | ((~md5->b) & md5->d);
		else if (i > 15 && i < 32)
			f = (md5->d & md5->b) | ((~md5->d) & md5->c);
		else if (i > 31 && i < 48)
			f = md5->b ^ md5->c ^ md5->d;
		else if (i > 47 && i < 64)
			f = md5->c ^ (md5->b | (~md5->d));
		temp = md5->d;
		md5->d = md5->c;
		md5->c = md5->b;
		md5->b = md5->b + md5_left_rotate(f + md5->a +\
				g_md5_k[i] + msg[g_md5_m[i]], g_md5_s[i]);
		md5->a = temp;
		i++;
	}
	md5_addback(md5);
}

static void		md5_transform(t_md5 *md5)
{
	uint32_t	chunk;

	chunk = 0;
	while (chunk < md5->msg_len)
	{
		md5_arg((uint32_t *)(md5->msg + chunk), md5);
		chunk = chunk + 64;
	}
}

static void		md5_padding(uint8_t *msg, size_t len, t_md5 *md5)
{
	size_t		i;

	md5->a0 = 0x67452301;
	md5->b0 = 0xefcdab89;
	md5->c0 = 0x98badcfe;
	md5->d0 = 0x10325476;
	md5->msg_len = len * 8 + 1;
	while (md5->msg_len % 512 != 448)
		md5->msg_len++;
	if (!(md5->msg = ft_memalloc(md5->msg_len + 64)))
		return ;
	ft_strcpy((char *)md5->msg, (const char *)msg);
	md5->msg_len /= 8;
	i = len;
	md5->msg[i] = 128;
	while (++i < md5->msg_len)
		md5->msg[i] = 0;
	*(uint32_t*)(md5->msg + i) = (uint32_t)len * 8;
}

char	*hash_md5(char *s)
{
	t_md5		md5;
	size_t len;

	len = ft_strlen(s);
	ft_bzero(&md5, sizeof(t_md5));
	md5_padding(s, len, &md5);
	md5_transform(&md5);
	md5_print(&md5);
	ft_memdel((void **)&(md5.msg));
	
	return ("0");
}
