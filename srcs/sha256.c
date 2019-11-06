/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsimon <lsimon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 10:54:27 by lsimon            #+#    #+#             */
/*   Updated: 2019/11/06 14:05:41 by lsimon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ssl.h"

#define UINT32_T_SIZE sizeof(uint32_t)
#define UINT32_T_BITS (UINT32_T_SIZE * 8 - 1)

static uint32_t	to_big_endian (uint32_t num)
{
	return (
		((num>>24)&0xff) | // move byte 3 to byte 0
		((num<<8)&0xff0000) | // move byte 1 to byte 2
		((num>>8)&0xff00) | // move byte 2 to byte 1
		((num<<24)&0xff000000) // byte 0 to byte 3
	);
}

static uint32_t rotateRight(uint32_t num, unsigned int rotation)
{
    uint32_t DROPPED_LSB;

    rotation %= UINT32_T_BITS;
    while(rotation--)
    {
        DROPPED_LSB = num & 1;
        num = (num >> 1) & (~(1 << UINT32_T_BITS));
        num = num | (DROPPED_LSB << UINT32_T_BITS);
	}

    return num;
}

/*
**Initialize hash values:
**(first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):
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
	uint64_t	*uint64_t_content;

	message_len = ft_strlen(s);
	L = message_len * 8;
	K = get_k(L);

	padded.byte_size = (L + 1 + 64 + K) / 8;
	padded.content = malloc(padded.byte_size);
	ft_bzero(padded.content, padded.byte_size);
	padded.n_chunks = (L + K + 64 + 1) / 512;

	
	char_content = padded.content;
	ft_memcpy(padded.content, s, message_len);
	char_content[message_len] |= 1 << 0;
	printf("(debug) char_content: %s\n", char_content);

	uint64_t_content = padded.content;
	uint64_t_content[(padded.byte_size / 2) - 1] = (uint64_t)L;
	printf("(debug) char_content: %s\n", char_content);
	return (padded);
}

/*
** break message into 512-bit chunks
*/
static uint32_t	**message_to_chunks(uint32_t *padded_content, size_t n_chunks)
{
	uint32_t	**chunks;
	size_t		i;

	printf("(debug) SPLITING INTO %zu 512 BITS CHUNKS\n", n_chunks);
	chunks = malloc(n_chunks);
	i = 0;
	while (i < n_chunks)
	{
		chunks[i] = padded_content + ((64 / 8) * i);
		i++;
	}
	return (chunks);
}

/*
** create a 64-entry message schedule array w[0..63] of 32-bit words
*/
// static uint32_t	**chunk_to_words(uint32_t *chunk)
// {
// }

/*
** (The initial values in w[0..63] don't matter, so many implementations zero them here)
** copy chunk into first 16 words w[0..15] of the message schedule array
** 
** Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array:
** for i from 16 to 63
**     s0 := (w[i-15] rightrotate  7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift  3)
**     s1 := (w[i- 2] rightrotate 17) xor (w[i- 2] rightrotate 19) xor (w[i- 2] rightshift 10)
**     w[i] := w[i-16] + s0 + w[i-7] + s1
*/
static uint32_t *preprocess(uint32_t *chunk)
{
	uint32_t	*w;
	size_t		i;
	uint32_t	s0;
	uint32_t	s1;

	printf("(debug) create a 64-entry message schedule array w[0..63] of 32-bit words\n");
	w = malloc(64 * sizeof(*w));
	i = 0;
	while (i < 16)
	{
		w[i] = to_big_endian(chunk[i]);
		printf("(debug) w[i]: %x\n", w[i]);
		i++;
	}
	printf("(debug) PREPROCESS, PHASE 2\n");
	while (i < 64)
	{
		s0 = (rotateRight(w[i - 15], 7) ^ (rotateRight(w[i - 15], 18)) ^ (w[i - 15] >> 3));
		s1 = (rotateRight(w[i - 2], 17) ^ (rotateRight(w[i - 2], 19) ^ (w[i - 2] >> 10)));
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		i++;
	}
	return (w);
}

/*
** Initialize working variables to current hash value:
**    a := h0
**    b := h1
**    c := h2
**    d := h3
**    e := h4
**    f := h5
**    g := h6
**    h := h7
**
**    Compression function main loop:
**    for i from 0 to 63
**        S1 := (e rightrotate 6) xor (e rightrotate 11) xor (e rightrotate 25)
**        ch := (e and f) xor ((not e) and g)
**        temp1 := h + S1 + ch + k[i] + w[i]
**        S0 := (a rightrotate 2) xor (a rightrotate 13) xor (a rightrotate 22)
**        maj := (a and b) xor (a and c) xor (b and c)
**        temp2 := S0 + maj
** 
**        h := g
**        g := f
**        f := e
**        e := d + temp1
**        d := c
**        c := b
**        b := a
**        a := temp1 + temp2
**
**    Add the compressed chunk to the current hash value:
**    h0 := h0 + a
**    h1 := h1 + b
**    h2 := h2 + c
**    h3 := h3 + d
**    h4 := h4 + e
**    h5 := h5 + f
**    h6 := h6 + g
**    h7 := h7 + h
**
*/
void	compress(uint32_t *w)
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

	printf("(debug) COMPRESS\n");
	i = 0;
	while (i < 64)
	{
		uint32_t	s1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
		uint32_t	ch = (e & f) ^ ((~e) & g);
		uint32_t	temp1 = h + s1 + ch + g_k[i] + w[i];
		uint32_t	s0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
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

void	process_chunks(uint32_t **chunks, size_t n_chunks, size_t i)
{
	uint32_t	*w;

	if (i == n_chunks)
		return ;
	w = preprocess(chunks[i]);
	compress(w);
	process_chunks(chunks, n_chunks, i + 1);
}

static char	*ft_itoa_base_aux(unsigned int n, unsigned int base, const char *base_s, char *acc)
{
	char			*s;

	s = malloc(ft_strlen(acc) + 1);
	ft_bzero(s, ft_strlen(acc) + 1);
	s[0] = base_s[n % base];
	ft_strcat(s, acc);
	free(acc);
	if (n < base)
		return (s);
	else
		return (ft_itoa_base_aux(n / base, base, base_s, s));
}

char		*ft_itoa_base(unsigned int n, unsigned int base)
{
	char		*s;

	s = ft_itoa_base_aux(n, base, "0123456789abcdef", ft_strdup(""));
	return (s);
}
/*
** Produce the final hash value (big-endian):
** digest := hash := h0 append h1 append h2 append h3 append h4 append h5 append h6 append h7
*/
static char	*digest(void)
{
	char	*hash;

	printf("(debug) DIGEST\n");
	hash = malloc(1000);
	ft_strcpy(hash, ft_itoa_base(g_h0, 16));
	ft_strcat(hash, ft_itoa_base(g_h1, 16));
	ft_strcat(hash, ft_itoa_base(g_h2, 16));
	ft_strcat(hash, ft_itoa_base(g_h3, 16));
	ft_strcat(hash, ft_itoa_base(g_h4, 16));
	ft_strcat(hash, ft_itoa_base(g_h5, 16));
	ft_strcat(hash, ft_itoa_base(g_h6, 16));
	ft_strcat(hash, ft_itoa_base(g_h7, 16));
	return (hash);
}

char	*hash_sha256(char *message)
{
	t_mem		padded;
	uint32_t	**chunks;

	(void)message;
	printf("(debug) g_h0: %u\n", g_h0);
	printf("(debug) g_k[0]: %u\n", g_k[0]);
	padded = pad("abc");
	chunks = message_to_chunks(padded.content, padded.n_chunks);
	process_chunks(chunks, padded.n_chunks, 0);
	free(padded.content);
	return (digest());
}
