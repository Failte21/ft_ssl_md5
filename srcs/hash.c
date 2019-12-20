/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 10:46:16 by lsimon            #+#    #+#             */
/*   Updated: 2019/12/20 10:46:17 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

static t_hash_fn	get_hash_fn_aux(char *hash, unsigned int i)
{
	if (g_hash_table[i].hash == NULL)
		return (NULL);
	if (ft_strcmp(g_hash_table[i].hash, hash) == 0)
		return (g_hash_table[i].hash_fn);
	return (get_hash_fn_aux(hash, i + 1));
}

t_hash_fn			get_hash_fn(char *hash)
{
	t_hash_fn		hash_fn;

	if (hash == NULL)
	{
		arg_missing();
		return (NULL);
	}
	if ((hash_fn = (get_hash_fn_aux(hash, 0))) == NULL)
		arg_invalid_command(hash);
	return (hash_fn);
}
