#include "../inc/ft_ssl.h"

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

t_mem			sha256_pad(t_content *c)
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
