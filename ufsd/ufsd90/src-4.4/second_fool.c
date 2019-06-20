#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <asm/param.h>			/* for HZ */

extern unsigned long __msecs_to_jiffies(const unsigned int m);

#define MAX_JIFFY_OFFSET ((LONG_MAX >> 1)-1)

#if HZ <= MSEC_PER_SEC && !(MSEC_PER_SEC % HZ)
/*
 * HZ is equal to or smaller than 1000, and 1000 is a nice round
 * multiple of HZ, divide with the factor between them, but round
 * upwards:
 */
static inline unsigned long _msecs_to_jiffies(const unsigned int m)
{
	return (m + (MSEC_PER_SEC / HZ) - 1) / (MSEC_PER_SEC / HZ);
}
#elif HZ > MSEC_PER_SEC && !(HZ % MSEC_PER_SEC)
/*
 * HZ is larger than 1000, and HZ is a nice round multiple of 1000 -
 * simply multiply with the factor between them.
 *
 * But first make sure the multiplication result cannot overflow:
 */
static inline unsigned long _msecs_to_jiffies(const unsigned int m)
{
	if (m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;
	return m * (HZ / MSEC_PER_SEC);
}
#else
/*
 * Generic case - multiply, round and divide. But first check that if
 * we are doing a net multiplication, that we wouldn't overflow:
 */
static inline unsigned long _msecs_to_jiffies(const unsigned int m)
{
	if (HZ > MSEC_PER_SEC && m > jiffies_to_msecs(MAX_JIFFY_OFFSET))
		return MAX_JIFFY_OFFSET;

	return (MSEC_TO_HZ_MUL32 * m + MSEC_TO_HZ_ADJ32) >> MSEC_TO_HZ_SHR32;
}
#endif

unsigned long msecs_to_jiffies(const unsigned int m)
{
	if (__builtin_constant_p(m)) {
		if ((int)m < 0)
			return MAX_JIFFY_OFFSET;
		return _msecs_to_jiffies(m);
	} else {
		return __msecs_to_jiffies(m);
	}
}
//EXPORT_SYMBOL(msecs_to_jiffies);

