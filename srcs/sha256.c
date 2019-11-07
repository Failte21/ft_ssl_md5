#include "../inc/ft_ssl.h"

/*
** Initialize hash values:
** (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
*/
static uint32_t	g_h0 = 0x6a09e667;
static uint32_t	g_h1 = 0xbb67ae85;
static uint32_t	g_h2 = 0x3c6ef372;
static uint32_t	g_h3 = 0xa54ff53a;
static uint32_t	g_h4 = 0x510e527f;
static uint32_t	g_h5 = 0x9b05688c;
static uint32_t	g_h6 = 0x1f83d9ab;
static uint32_t	g_h7 = 0x5be0cd19;

static unsigned int	g_k[] =
{
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/*
** Pre-processing (Padding):
** begin with the original message of length L bits
** append a single '1' bit
** append K '0' bits, where K is the minimum number >= 0 such that L + 1 + K + 64 is a multiple of 512
** append L as a 64-bit big-endian integer, making the total post-processed length a multiple of 512 bits
*/

static uint32_t	get_k(uint32_t L)
{
	uint32_t	mod;

	mod = ((L + 1 + 64) %  512);
	return (mod == 0 ? 0 : 512 - mod);
}

static t_mem 	pad(char *s)
{
	uint32_t	L;
	uint32_t	K;
	t_mem		padded;
	size_t		message_len;
	char		*char_content;

	message_len = ft_strlen(s);
	L = message_len * 8;
	K = get_k(L);

	padded.byte_size = (L + 1 + 64 + K) / 8;
	padded.content = malloc(padded.byte_size);
	ft_bzero(padded.content, padded.byte_size);
	padded.n_chunks = (L + K + 64 + 1) / 512;

	
	char_content = padded.content;
	ft_memcpy(padded.content, s, message_len);
	char_content[message_len] = 0x80;

	uint64_t bitlen = message_len * 8;
	char_content[padded.byte_size - 1] = bitlen;
	char_content[padded.byte_size - 2] = bitlen >> 8;
	char_content[padded.byte_size - 3] = bitlen >> 16;
	char_content[padded.byte_size - 4] = bitlen >> 24;
	char_content[padded.byte_size - 5] = bitlen >> 32;
	char_content[padded.byte_size - 6] = bitlen >> 40;
	char_content[padded.byte_size - 7] = bitlen >> 48;
	char_content[padded.byte_size - 8] = bitlen >> 56;

	return (padded);
}

static uint32_t *preprocess(uint32_t *chunk)
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
		s0 = (rot_right(w[i - 15], 7) ^ (rot_right(w[i - 15], 18)) ^ (w[i - 15] >> 3));
		s1 = (rot_right(w[i - 2], 17) ^ (rot_right(w[i - 2], 19) ^ (w[i - 2] >> 10)));
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		i++;
	}
	return (w);
}

static void		compress(uint32_t *w)
{
	uint32_t	a = g_h0;
	uint32_t	b = g_h1;
	uint32_t	c = g_h2;
	uint32_t	d = g_h3;
	uint32_t	e = g_h4;
	uint32_t	f = g_h5;
	uint32_t	g = g_h6;
	uint32_t	h = g_h7;

	size_t		i;

	i = 0;
	while (i < 64)
	{
		uint32_t	s1 = rot_right(e, 6) ^ rot_right(e, 11) ^ rot_right(e, 25);
		uint32_t	ch = (e & f) ^ ((~e) & g);
		uint32_t	temp1 = h + s1 + ch + g_k[i] + w[i];
		uint32_t	s0 = rot_right(a, 2) ^ rot_right(a, 13) ^ rot_right(a, 22);
		uint32_t	maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t	temp2 = s0 + maj;

		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
		i++;
	}
	g_h0 = g_h0 + a;
	g_h1 = g_h1 + b;
	g_h2 = g_h2 + c;
	g_h3 = g_h3 + d;
	g_h4 = g_h4 + e;
	g_h5 = g_h5 + f;
	g_h6 = g_h6 + g;
	g_h7 = g_h7 + h;
}

static void		process_chunks(uint32_t **chunks, size_t n_chunks, size_t i)
{
	uint32_t	*w;

	if (i == n_chunks)
		return ;
	w = preprocess(chunks[i]);
	compress(w);
	process_chunks(chunks, n_chunks, i + 1);
}

/*
** Hacky way to append potentially missing 0s
*/
static char		*adjust(char *hash)
{
	unsigned int	to_add;
	char			*adjusted;

	to_add = 64 - ft_strlen(hash);
	adjusted = malloc(65);
	ft_memset(adjusted, '0', to_add);
	ft_strcpy(adjusted + to_add, hash);
	free(hash);
	return (adjusted);
}

/*
** Produce the final hash value (big-endian):
*/
static char		*digest(void)
{
	char	*hash;

	hash = malloc(1000);
	ft_strcpy(hash, ft_itoa_base_u(g_h0, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h1, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h2, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h3, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h4, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h5, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h6, 16));
	ft_strcat(hash, ft_itoa_base_u(g_h7, 16));
	return (adjust(hash));
}

char			*hash_sha256(char *message)
{
	t_mem		padded;
	uint32_t	**chunks;

	padded = pad(message);
	chunks = message_to_chunks(padded.content, padded.n_chunks);
	process_chunks(chunks, padded.n_chunks, 0);
	free(padded.content);
	return (digest());
}
