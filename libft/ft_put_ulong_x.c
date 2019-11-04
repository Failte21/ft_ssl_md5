/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_ulong_x.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 10:04:50 by lsimon            #+#    #+#             */
/*   Updated: 2018/12/03 09:52:29 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"

void	ft_put_ulong_x(unsigned long value)
{
	char const *base = "0123456789abcdef";

	if (value / 16 > 0)
		ft_put_ulong_x(value / 16);
	write(1, base + value % 16, 1);
}
