/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:22:26 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/04 15:19:21 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

char				*get_to_hash(int fd)
{
	char			buf[BUF_SIZE];
	char			*to_hash;
	char			*tmp;
	int				r;

	to_hash = NULL;
	while ((r = read(fd, buf, BUF_SIZE - 1)))
	{
		buf[r] = '\0';
 		if (to_hash == NULL)
			to_hash = ft_strdup(buf);
		else
		{
			tmp = to_hash;
			to_hash = malloc(ft_strlen(tmp) + ft_strlen(buf) + 1);
			ft_strcpy(to_hash, tmp);
			ft_strcat(to_hash, buf);
			free(tmp);
		}
	}
	return (to_hash);
}

int					get_file_content(t_handler *handler, char *filepath)
{
	int		fd;
	char	*to_hash;

	fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		printf("(debug): open error %s\n", strerror(errno));
		return (-1);
	}
	if ((to_hash = get_to_hash(fd)) == NULL)
	{
		printf("(debug) error while reading the file\n");
		return (-1);
	}
	printf("(debug) FILE CONTENT: to_hash: %s\n", to_hash);
	handler->to_hash = to_hash;
	return (0);
}

void				quiet_mode(t_handler *handler)
{
	handler->quiet = true;
}

void				verbose_mode(t_handler *handler)
{
	handler->verbose = true;
}

void				reversed_mode(t_handler *handler)
{
	handler->reversed = true;
}

int					string_mode(t_handler *handler, char *s)
{
	if (s == NULL)
		return (-1);
	handler->to_hash = s;
	printf("(debug) STRING MODE s: %s\n", s);
	return (0);
}

static t_flag_fn	get_flag_fn_aux(char c, unsigned int i)
{
	if (g_flag_handlers[i].flag == 0)
	{
		ft_putstr_fd("Unknown flag: ", 1);
		ft_putchar_fd(c, 1);
		ft_putchar_fd('\n', 1);
		return (NULL);
	}
	if (g_flag_handlers[i].flag == c)
		return (g_flag_handlers[i].flag_fn);
	return (get_flag_fn_aux(c, i + 1));
}

t_flag_fn			get_flag_fn(char c)
{
	return get_flag_fn_aux(c, 0);
}

int					handle_file(t_handler *handler, char **args)
{
	if (*args != NULL)
	{
		if (handler->to_hash != NULL)
		{
			printf("(debug) conflict s flag and file\n");
			return (-1);
		}
		if (*(args + 1) != NULL)
		{
			printf("(debug) Error too many args\n");
			return (-1);
		}
		return (get_file_content(handler, *args));
	}
	return (0);
}

int					handle_flags_aux(t_handler *handler, char **args, unsigned int i)
{
	unsigned int	j;
	t_flag_fn		flag_fn;

	j = 1;
	printf("(debug) args[i]: %s\n", args[i]);
	if (args[i] == NULL)
		return (0);
	if (*(args[i]) == '-')
	{
		if (ft_strcmp(args[i], "-s") == 0)
		{
			i += 1;
			if ((string_mode(handler, args[i])) == -1)
				return (-1);
		}
		else
		{
			while (args[i][j] != '\0')
			{
				if ((flag_fn = get_flag_fn(args[i][j])) == NULL)
					return (-1);
				flag_fn(handler);
			}
		}
		return (handle_flags_aux(handler, args, i + 1));
	}
	return (handle_file(handler, args + i));
}

int					handle_flags(t_handler *handler, char **args)
{
	return (handle_flags_aux(handler, args, 0));
}

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
		printf("(debug) Missing hash argument\n");
		return (NULL);
	}
	if ((hash_fn = (get_hash_fn_aux(hash, 0))) == NULL)
		printf("(debug) Invalid hash argument\n");
	return (hash_fn);
}

t_handler			*init_handler(int ac, char **av)
{
	t_handler		*handler;
	t_hash_fn		hash_fn;

	(void)ac;
	if ((hash_fn = get_hash_fn(av[1])) == NULL)
		return (NULL);
	handler = malloc(sizeof(t_handler));
	if (handler == NULL)
	{
		ft_putstr_fd("Error allocating memory for handler structure", 1);
		return (NULL);
	}
	handler->hash_fn = hash_fn;
	handler->flags = NULL;
	handler->to_hash = NULL;
	return (handler);
}

int					main(int ac, char **av)
{
	t_handler	*handler;
	char		*hashed;

	handler = init_handler(ac, av);
	if (handler == NULL)
		return (1);
	if (handle_flags(handler, av + 2) == -1)
	{
		free(handler);
		return (-1);
	}
	handler->to_hash = handler->to_hash == NULL ?
		get_to_hash(0) :
		handler->to_hash;
	if (handler->to_hash == NULL)
	{
		free(handler);
		return (1);
	}
	if ((hashed = handler->hash_fn(handler->to_hash)) == NULL)
	{
		free(handler);
		return (1);
	}
	printf("(debug) hashed: %s\n", hashed);
	return (0);
}
