/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_put_byte_base.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/17 10:04:50 by lsimon            #+#    #+#             */
/*   Updated: 2018/12/04 12:28:57 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "unistd.h"

void	ft_put_byte_base(char byte, unsigned int ibase)
{
	char const		*base = "0123456789abcdef";

	if (byte / ibase > 0)
		ft_put_byte_base(byte / ibase, ibase);
	write(1, base + byte % ibase, 1);
}
