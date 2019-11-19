#include "../inc/ft_ssl.h"

uint32_t	rot_right(uint32_t a, size_t b)
{
	return (((a) >> (b)) | ((a) << (32 - (b))));
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

char		*ft_itoa_hex_u_fixed(unsigned int n)
{
	char		*s;
	char		*adjusted;
	size_t		to_add;

	s = ft_itoa_base_u(n, 16);
	to_add = 8 - ft_strlen(s);
	adjusted = malloc(8);
	ft_memset(adjusted, '0', to_add);
	ft_strcpy(adjusted + to_add, s);
	free(s);
	return (adjusted);
}
