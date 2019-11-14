#include "../inc/ft_ssl.h"

/*
** Initialize hash values:
** (first 32 bits of the fractional parts of the square roots
** of the first 8 primes 2..19):
*/

static uint32_t	g_h[] =
{
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
	0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

static unsigned int	g_k[] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static uint32_t	get_k(uint32_t l)
{
	uint32_t	mod;

	mod = (l + 1 + 64) % 512;
	return (mod == 0 ? 0 : 512 - mod);
}

static char		*append_msg_len(char *s, size_t size, uint64_t bitlen)
{
	s[size - 1] = bitlen;
	s[size - 2] = bitlen >> 8;
	s[size - 3] = bitlen >> 16;
	s[size - 4] = bitlen >> 24;
	s[size - 5] = bitlen >> 32;
	s[size - 6] = bitlen >> 40;
	s[size - 7] = bitlen >> 48;
	s[size - 8] = bitlen >> 56;
	return (s);
}

/*
** Pre-processing (Padding):
** begin with the original message of length L bits
** append a single '1' bit
** append K '0' bits, where K is the minimum number >= 0 such
** that L + 1 + K + 64 is a multiple of 512
** append L as a 64-bit big-endian integer,
** making the total post-processed length a multiple of 512 bits
*/

static t_mem	pad(t_content *c)
{
	uint32_t	l;
	uint32_t	k;
	t_mem		padded;
	size_t		message_len;
	char		*char_content;

	message_len = c->size;
	l = message_len * 8;
	k = get_k(l);
	padded.byte_size = (l + 1 + 64 + k) / 8;
	padded.content = malloc(padded.byte_size);
	ft_bzero(padded.content, padded.byte_size);
	padded.n_chunks = (l + k + 64 + 1) / 512;
	char_content = (char *)padded.content;
	ft_memcpy(padded.content, c->content, message_len);
	char_content[message_len] = 0x80;
	append_msg_len(char_content, padded.byte_size, message_len * 8);
	return (padded);
}

static uint32_t	*preprocess(uint32_t *chunk)
{
	uint32_t	*w;
	size_t		i;
	uint32_t	s0;
	uint32_t	s1;

	w = malloc(64 * sizeof(*w));
	i = 0;
	while (i < 16)
	{
		w[i] = to_big_endian(chunk[i]);
		i++;
	}
	while (i < 64)
	{
		s0 = (rot_right(w[i - 15], 7) ^ (rot_right(w[i - 15], 18))
			^ (w[i - 15] >> 3));
		s1 = (rot_right(w[i - 2], 17) ^ (rot_right(w[i - 2], 19)
			^ (w[i - 2] >> 10)));
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		i++;
	}
	return (w);
}

static void		fill_a(uint32_t a[8])
{
	size_t	i;

	i = 0;
	while (i < 8)
	{
		a[i] = g_h[i];
		i++;
	}
}

static void		fill_temp(uint32_t tmp[2], uint32_t a[8], size_t i, uint32_t *w)
{
	uint32_t	s1;
	uint32_t	ch;
	uint32_t	s0;
	uint32_t	maj;

	s1 = rot_right(a[4], 6) ^ rot_right(a[4], 11) ^ rot_right(a[4], 25);
	ch = (a[4] & a[5]) ^ ((~a[4]) & a[6]);
	tmp[0] = a[7] + s1 + ch + g_k[i] + w[i];
	s0 = rot_right(a[0], 2) ^ rot_right(a[0], 13) ^ rot_right(a[0], 22);
	maj = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]);
	tmp[1] = s0 + maj;
}

static void		update_as(uint32_t as[8], uint32_t temp[2])
{
	as[7] = as[6];
	as[6] = as[5];
	as[5] = as[4];
	as[4] = as[3] + temp[0];
	as[3] = as[2];
	as[2] = as[1];
	as[1] = as[0];
	as[0] = temp[0] + temp[1];
}

static void		update_hs(uint32_t as[8])
{
	g_h[0] = g_h[0] + as[0];
	g_h[1] = g_h[1] + as[1];
	g_h[2] = g_h[2] + as[2];
	g_h[3] = g_h[3] + as[3];
	g_h[4] = g_h[4] + as[4];
	g_h[5] = g_h[5] + as[5];
	g_h[6] = g_h[6] + as[6];
	g_h[7] = g_h[7] + as[7];
}

static void		compress(uint32_t *w)
{
	size_t		i;
	uint32_t	as[8];
	uint32_t	temp[2];

	fill_a(as);
	i = 0;
	while (i < 64)
	{
		fill_temp(temp, as, i, w);
		update_as(as, temp);
		i++;
	}
	update_hs(as);
}

/*
** Produce the final hash value (big-endian):
*/

static char		*digest(void)
{
	char	*hash;
	char	*tmp;
	size_t	i;

	hash = malloc(8 * 8 + 1);
	tmp = ft_itoa_hex_u_fixed(g_h[0]);
	ft_strcpy(hash, tmp);
	free(tmp);
	i = 1;
	while (i < 8)
	{
		tmp = ft_itoa_hex_u_fixed(g_h[i]);
		ft_strcat(hash, tmp);
		free(tmp);
		i++;
	}
	return (hash);
}

char			*hash_sha256(t_content *content)
{
	t_mem		padded;
	size_t		i;
	uint32_t	*w;

	padded = pad(content);
	i = 0;
	while (i < padded.n_chunks)
	{
		w = preprocess(((uint32_t *)padded.content) + (16 * i));
		compress(w);
		free(w);
		i++;
	}
	free(padded.content);
	return (digest());
}
