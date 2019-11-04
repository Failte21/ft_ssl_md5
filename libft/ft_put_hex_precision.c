/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_hex_precision.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 09:49:15 by lsimon            #+#    #+#             */
/*   Updated: 2018/12/08 13:15:11 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"

void	print(unsigned long value, unsigned int i, unsigned int precision)
{
	char const *base = "0123456789abcdef";

	if (value / 16 > 0)
		print(value / 16, i + 1, precision);
	if (i < precision)
		write(1, base + value % 16, 1);
}

void	ft_put_hex_precision(unsigned long value, unsigned int precision)
{
	unsigned int len;

	len = ft_hex_len(value);
	if (len < precision)
		write(1, "00000000000000000000000000000000", precision - len);
	print(value, 0, precision);
}
