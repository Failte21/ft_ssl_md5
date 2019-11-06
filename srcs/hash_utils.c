#include "../inc/ft_ssl.h"

uint32_t	to_big_endian(uint32_t num)
{
	return (
		((num >> 24) & 0xff) |
		((num << 8) & 0xff0000) |
		((num >> 8) & 0xff00) |
		((num << 24) & 0xff000000));
}

uint32_t	rot_right(uint32_t a, size_t b)
{
	return (((a) >> (b)) | ((a) << (32 - (b))));
}

/*
** break message into 512-bit chunks
*/

uint32_t	**message_to_chunks(uint32_t *content, size_t n_chunks)
{
	uint32_t	**chunks;
	size_t		i;

	chunks = malloc(n_chunks);
	i = 0;
	while (i < n_chunks)
	{
		chunks[i] = content + ((64 / 8) * i);
		i++;
	}
	return (chunks);
}

static char	*ft_itoa_base_u_aux(unsigned int n, unsigned int base, char *acc)
{
	char			*s;
	const char		*base_s = "0123456789abcdef";

	s = malloc(ft_strlen(acc) + 1);
	ft_bzero(s, ft_strlen(acc) + 1);
	s[0] = base_s[n % base];
	ft_strcat(s, acc);
	free(acc);
	if (n < base)
		return (s);
	else
		return (ft_itoa_base_u_aux(n / base, base, s));
}

char		*ft_itoa_base_u(unsigned int n, unsigned int base)
{
	char		*s;

	s = ft_itoa_base_u_aux(n, base, ft_strdup(""));
	return (s);
}
