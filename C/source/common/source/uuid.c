#include "uuid.h"
#include "uuidp.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include <sys/syscall.h>
#include <sys/stat.h>

int random_get_fd(void)
{
	int i, fd;
	struct timeval	tv;

	gettimeofday(&tv, 0);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		fd = open("/dev/random", O_RDONLY | O_NONBLOCK);
	if (fd >= 0) {
		i = fcntl(fd, F_GETFD);
		if (i >= 0)
			fcntl(fd, F_SETFD, i | FD_CLOEXEC);
	}
	srand((getpid() << 16) ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);

#ifdef DO_JRAND_MIX
	ul_jrand_seed[0] = getpid() ^ (tv.tv_sec & 0xFFFF);
	ul_jrand_seed[1] = getppid() ^ (tv.tv_usec & 0xFFFF);
	ul_jrand_seed[2] = (tv.tv_sec ^ tv.tv_usec) >> 16;
#endif
	/* Crank the random number generator a few times */
	gettimeofday(&tv, 0);
	for (i = (tv.tv_sec ^ tv.tv_usec) & 0x1F; i > 0; i--)
		rand();
	return fd;
}

void random_get_bytes(void *buf, size_t nbytes)
{
	size_t i, n = nbytes;
	int fd = random_get_fd();
	int lose_counter = 0;
	unsigned char *cp = (unsigned char *) buf;

	if (fd >= 0) {
		while (n > 0) {
			ssize_t x = read(fd, cp, n);
			if (x <= 0) {
				if (lose_counter++ > 16)
					break;
				continue;
			}
			n -= x;
			cp += x;
			lose_counter = 0;
		}

		close(fd);
	}

	/*
	 * We do this all the time, but this is the only source of
	 * randomness if /dev/random/urandom is out to lunch.
	 */
	for (cp = buf, i = 0; i < nbytes; i++)
		*cp++ ^= (rand() >> 7) & 0xFF;

#ifdef DO_JRAND_MIX
	{
		unsigned short tmp_seed[3];

		memcpy(tmp_seed, ul_jrand_seed, sizeof(tmp_seed));
		ul_jrand_seed[2] = ul_jrand_seed[2] ^ syscall(__NR_gettid);
		for (cp = buf, i = 0; i < nbytes; i++)
			*cp++ ^= (jrand48(tmp_seed) >> 7) & 0xFF;
		memcpy(ul_jrand_seed, tmp_seed,
		       sizeof(ul_jrand_seed)-sizeof(unsigned short));
	}
#endif

	return;
}

void uuid_unpack(const uuid_t in, struct uuid *uu)
{
	const uint8_t	*ptr = in;
	uint32_t		tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_low = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_mid = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->time_hi_and_version = tmp;

	tmp = *ptr++;
	tmp = (tmp << 8) | *ptr++;
	uu->clock_seq = tmp;

	memcpy(uu->node, ptr, 6);
}

void uuid_pack(const struct uuid *uu, uuid_t ptr)
{
	uint32_t	tmp;
	unsigned char	*out = ptr;

	tmp = uu->time_low;
	out[3] = (unsigned char) tmp;
	tmp >>= 8;
	out[2] = (unsigned char) tmp;
	tmp >>= 8;
	out[1] = (unsigned char) tmp;
	tmp >>= 8;
	out[0] = (unsigned char) tmp;

	tmp = uu->time_mid;
	out[5] = (unsigned char) tmp;
	tmp >>= 8;
	out[4] = (unsigned char) tmp;

	tmp = uu->time_hi_and_version;
	out[7] = (unsigned char) tmp;
	tmp >>= 8;
	out[6] = (unsigned char) tmp;

	tmp = uu->clock_seq;
	out[9] = (unsigned char) tmp;
	tmp >>= 8;
	out[8] = (unsigned char) tmp;

	memcpy(out+10, uu->node, 6);
}

void __uuid_generate_random(uuid_t out, int *num)
{
	uuid_t	buf;
	struct uuid uu;
	int i, n;

	if (!num || !*num)
		n = 1;
	else
		n = *num;

	for (i = 0; i < n; i++) {
		random_get_bytes(buf, sizeof(buf));
		uuid_unpack(buf, &uu);

		uu.clock_seq = (uu.clock_seq & 0x3FFF) | 0x8000;
		uu.time_hi_and_version = (uu.time_hi_and_version & 0x0FFF)
			| 0x4000;
		uuid_pack(&uu, out);
		out += sizeof(uuid_t);
	}
}

void uuid_generate_random(uuid_t out)
{
	int	num = 1;
	/* No real reason to use the daemon for random uuid's -- yet */

	__uuid_generate_random(out, &num);
}



/*
 * Check whether good random source (/dev/random or /dev/urandom)
 * is available.
 */
static int have_random_source(void)
{
	struct stat s;

	return (!stat("/dev/random", &s) || !stat("/dev/urandom", &s));
}


void uuid_generate(uuid_t out)
{
    if (have_random_source())
		uuid_generate_random(out);
	else
		;//uuid_generate_time(out);
}