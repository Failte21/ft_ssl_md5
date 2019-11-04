/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_hex_swap.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/03 11:44:40 by lsimon            #+#    #+#             */
/*   Updated: 2018/12/08 13:15:01 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_put_hex_swap(unsigned long value)
{
	char const *base = "0123456789abcdef";

	write(1, base + value % 16, 1);
	if (value / 16 > 0)
		ft_put_hex_swap(value / 16);
}
