/* md5.h
** interface to MD5 hash function
**
** This code is in the Public Domain.
*/

/** @file
 *  MD5 hash API.
 */

#ifndef MD5_H
#define MD5_H

/* we use the uint32_t type from stdint.h
 * if it is not available, add a typedef here:
 */
/* make sure the stdint.h types are available */
#if defined(_MSC_VER) /* Microsoft Visual C++ */
  typedef unsigned int              uint32_t;
#else
#include <stdint.h>
#endif

struct MD5Context {
	uint32_t buf[4];
	uint32_t bits[2];
	unsigned char in[64];
};

void MD5Init(struct MD5Context *context);
void MD5Update(struct MD5Context *context, unsigned char const *buf,
	       uint32_t len);
void MD5Final(unsigned char digest[16], struct MD5Context *context);
void MD5Transform(uint32_t buf[4], const unsigned char in[64],
			struct MD5Context *ctx);

#ifdef DEBUG_MD5
void MD5DumpBytes(unsigned char *b, int len);
#endif

#endif /* !MD5_H */
