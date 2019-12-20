/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:10 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:11 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

static char		*md5_digest(t_md5_tools *md5)
{
	char	*hash;

	hash = malloc(1000);
	ft_strcpy(hash, ft_itoa_hex_u_fixed(md5->aa));
	ft_strcat(hash, ft_itoa_hex_u_fixed(md5->bb));
	ft_strcat(hash, ft_itoa_hex_u_fixed(md5->cc));
	ft_strcat(hash, ft_itoa_hex_u_fixed(md5->dd));
	return (hash);
}

static void		algorithm(uint32_t *s, t_md5_tools *md5)
{
	uint32_t	f_part;
	uint32_t	i;
	uint32_t	tmp;

	i = 0;
	init(md5);
	while (i < 64)
	{
		if (i < 16)
			f_part = (md5->b & md5->c) | ((~md5->b) & md5->d);
		else if (i > 15 && i < 32)
			f_part = (md5->d & md5->b) | ((~md5->d) & md5->c);
		else if (i > 31 && i < 48)
			f_part = md5->b ^ md5->c ^ md5->d;
		else if (i > 47 && i < 64)
			f_part = md5->c ^ (md5->b | (~md5->d));
		tmp = md5->d;
		md5->d = md5->c;
		md5->c = md5->b;
		md5->b = md5->b + left_rotate(f_part + md5->a +\
				g_k_part[i] + s[g_m_part[i]], g_s_part[i]);
		md5->a = tmp;
		i++;
	}
	add_save_values(md5);
}

static void		message_padding(uint8_t *s, size_t len, t_md5_tools *md5)
{
	size_t		index;

	md5->aa = 0x67452301;
	md5->bb = 0xefcdab89;
	md5->cc = 0x98badcfe;
	md5->dd = 0x10325476;
	md5->s_len = len * 8;
	while (md5->s_len % 512 != 448)
		md5->s_len++;
	if (!(md5->s = ft_memalloc(md5->s_len + 64)))
		return ;
	ft_memcpy((char *)md5->s, (const char *)s, len);
	md5->s_len /= 8;
	index = len;
	md5->s[index] = 128;
	while (++index < md5->s_len)
		md5->s[index] = 0;
	*(uint32_t*)(md5->s + index) = (uint32_t)len * 8;
}

static void		transform(t_md5_tools *md5)
{
	uint32_t	chunk;

	chunk = 0;
	while (chunk < md5->s_len)
	{
		algorithm((uint32_t *)(md5->s + chunk), md5);
		chunk = chunk + 64;
	}
}

char			*hash_md5(t_content *content)
{
	t_md5_tools			md5;

	ft_bzero(&md5, sizeof(t_md5_tools));
	message_padding(content->content, content->size, &md5);
	transform(&md5);
	fix_endian(&md5);
	ft_memdel((void **)&(md5.s));
	return (md5_digest(&md5));
}
