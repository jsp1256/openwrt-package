#define generic_write_sync generic_write_sync_orig
#define current_kernel_time current_kernel_time_orig
#define find_lock_page find_lock_page_orig
#define find_or_create_page find_or_create_page_orig

#include "fool.h"
#include <linux/irqflags.h>
#include <linux/page-flags.h>
#include <linux/gfp.h>



#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/gfp.h>
#include <linux/percpu-defs.h>
#include <linux/mm_types.h>
#include <linux/pagevec.h>
#include <linux/percpu-defs.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/slab_def.h>
#include <uapi/linux/uio.h>
#include <linux/uio.h>
#include <linux/sched.h>
#include <linux/numa.h>
#include <linux/writeback.h>
#include <linux/mount.h>
#undef generic_write_sync
#undef current_kernel_time
#undef find_lock_page
#undef find_or_create_page
#undef test_set_page_writeback

static DEFINE_PER_CPU(struct pagevec, lru_add_pvec);


extern int vfs_fsync_range(struct file *file, loff_t start, loff_t end,
			   int datasync);
extern ssize_t generic_file_read_iter(struct kiocb *, struct iov_iter *);
extern ssize_t iter_file_splice_write(struct pipe_inode_info *,
		struct file *, loff_t *, size_t, unsigned int);
extern struct timespec64 current_kernel_time64(void);
extern struct page *pagecache_get_page(struct address_space *mapping, pgoff_t offset,
		int fgp_flags, gfp_t cache_gfp_mask);
extern struct page *pagecache_get_page(struct address_space *mapping, pgoff_t offset,
		int fgp_flags, gfp_t cache_gfp_mask);
extern int file_remove_privs(struct file *file);
/*
extern struct buffer_head *__bread_gfp(struct block_device *,
				sector_t block, unsigned size, gfp_t gfp);
extern struct buffer_head *__getblk_gfp(struct block_device *bdev,
				sector_t block, unsigned size, gfp_t gfp);
*/


extern ssize_t generic_perform_write(struct file *, struct iov_iter *, loff_t);
extern void __pagevec_lru_add(struct pagevec *pvec);
extern ssize_t __vfs_read(struct file *, char __user *, size_t, loff_t *);
extern ssize_t __vfs_write(struct file *, const char __user *, size_t, loff_t *);


/**
 * __mips_clear_bit - Clears a bit in memory.  This is called by clear_bit() if
 * it cannot find a faster solution.
 * @nr: Bit to clear
 * @addr: Address to start counting from
 * from 4.4.167 source code
 */
void __mips_clear_bit(unsigned long nr, volatile unsigned long *addr)
{
	unsigned long *a = (unsigned long *)addr;
	unsigned bit = nr & SZLONG_MASK;
	unsigned long mask;
	unsigned long flags;

	a += nr >> SZLONG_LOG;
	mask = 1UL << bit;
	raw_local_irq_save(flags);
	*a &= ~mask;
	raw_local_irq_restore(flags);
}

/**
 * __mips_set_bit - Atomically set a bit in memory.  This is called by
 * set_bit() if it cannot find a faster solution.
 * @nr: the bit to set
 * @addr: the address to start counting from
 * from 4.4.167 source code
 */
void __mips_set_bit(unsigned long nr, volatile unsigned long *addr)
{
	unsigned long *a = (unsigned long *)addr;
	unsigned bit = nr & SZLONG_MASK;
	unsigned long mask;
	unsigned long flags;

	a += nr >> SZLONG_LOG;
	mask = 1UL << bit;
	raw_local_irq_save(flags);
	*a |= mask;
	raw_local_irq_restore(flags);
}

/**
 * __mips_test_and_set_bit - Set a bit and return its old value.  This is
 * called by test_and_set_bit() if it cannot find a faster solution.
 * @nr: Bit to set
 * @addr: Address to count from
 * from 4.4.167 source code
 */
int __mips_test_and_set_bit(unsigned long nr,
			    volatile unsigned long *addr)
{
	unsigned long *a = (unsigned long *)addr;
	unsigned bit = nr & SZLONG_MASK;
	unsigned long mask;
	unsigned long flags;
	int res;

	a += nr >> SZLONG_LOG;
	mask = 1UL << bit;
	raw_local_irq_save(flags);
	res = (mask & *a) != 0;
	*a |= mask;
	raw_local_irq_restore(flags);
	return res;
}

/**
 * __mips_test_and_set_bit_lock - Set a bit and return its old value.  This is
 * called by test_and_set_bit_lock() if it cannot find a faster solution.
 * @nr: Bit to set
 * @addr: Address to count from
 * from 4.4.167 source code
 */
int __mips_test_and_set_bit_lock(unsigned long nr,
				 volatile unsigned long *addr)
{
	unsigned long *a = (unsigned long *)addr;
	unsigned bit = nr & SZLONG_MASK;
	unsigned long mask;
	unsigned long flags;
	int res;

	a += nr >> SZLONG_LOG;
	mask = 1UL << bit;
	raw_local_irq_save(flags);
	res = (mask & *a) != 0;
	*a |= mask;
	raw_local_irq_restore(flags);
	return res;
}

/**
 * __mips_test_and_clear_bit - Clear a bit and return its old value.  This is
 * called by test_and_clear_bit() if it cannot find a faster solution.
 * @nr: Bit to clear
 * @addr: Address to count from
 * from 4.4.167 source code
 */
int __mips_test_and_clear_bit(unsigned long nr, volatile unsigned long *addr)
{
	unsigned long *a = (unsigned long *)addr;
	unsigned bit = nr & SZLONG_MASK;
	unsigned long mask;
	unsigned long flags;
	int res;

	a += nr >> SZLONG_LOG;
	mask = 1UL << bit;
	raw_local_irq_save(flags);
	res = (mask & *a) != 0;
	*a &= ~mask;
	raw_local_irq_restore(flags);
	return res;
}

int test_set_page_writeback(struct page *page)
{			
	return __test_set_page_writeback(page, false);
}

int generic_write_sync(struct file *file, loff_t pos, loff_t count)
{
	if (!(file->f_flags & O_DSYNC) && !IS_SYNC(file->f_mapping->host))
		return 0;
	return vfs_fsync_range(file, pos, pos + count - 1,
			       (file->f_flags & __O_SYNC) ? 0 : 1);
}

//设计generic_file_aio_read的替代函数，内部用generic_file_read_iter转接
int generic_segment_checks(const struct iovec *iov,
			unsigned long *nr_segs, size_t *count, int access_flags)
{
	unsigned long   seg;
	size_t cnt = 0;
	for (seg = 0; seg < *nr_segs; seg++) {
		const struct iovec *iv = &iov[seg];

		/*
		 * If any segment has a negative length, or the cumulative
		 * length ever wraps negative then return -EINVAL.
		 */
		cnt += iv->iov_len;
		if (unlikely((ssize_t)(cnt|iv->iov_len) < 0))
			return -EINVAL;
		if (access_ok(access_flags, iv->iov_base, iv->iov_len))
			continue;
		if (seg == 0)
			return -EFAULT;
		*nr_segs = seg;
		cnt -= iv->iov_len;	/* This segment is no good */
		break;
	}
	*count = cnt;
	return 0;
}

int generic_file_aio_read(struct kiocb *iocb, const struct iovec *iov,
		unsigned long nr_segs, loff_t pos)
{
	ssize_t retval;
	struct iov_iter i;
	size_t count = 0;
	
	retval = generic_segment_checks(iov, &nr_segs, &count, VERIFY_WRITE);
	if (retval)
		return retval;
	iov_iter_init(&i, READ, iov, nr_segs, count);
	return generic_file_read_iter(iocb, &i);
}


/*
  based on https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/diff/?id=8d0207652cbe27d1f962050737848e5ad4671958
  it will be replaced by iter_file_splice_write
  generic_file_splice_write removed since Linux kernel v3.16
*/
ssize_t generic_file_splice_write(struct pipe_inode_info *pipe, struct file *out, loff_t *ppos, size_t len, unsigned int flags)
{
	return iter_file_splice_write(pipe, out, ppos, len, flags);
}

ssize_t
generic_file_buffered_write(struct kiocb *iocb, const struct iovec *iov,
		unsigned long nr_segs, loff_t pos, loff_t *ppos,
		size_t count, ssize_t written)
{
	struct file *file = iocb->ki_filp;
	ssize_t status;
	struct iov_iter i;

	iov_iter_init(&i, WRITE, iov, nr_segs, count);
	status = generic_perform_write(file, &i, pos);

	if (likely(status >= 0)) {
		written += status;
		*ppos = pos + status;
  	}
	
	return written ? written : status;
}

struct timespec current_kernel_time(void)
{
	struct timespec64 now = current_kernel_time64();

	return timespec64_to_timespec(now);
}

struct page *find_lock_page(struct address_space *mapping,
					pgoff_t offset)
{
	return pagecache_get_page(mapping, offset, FGP_LOCK, 0);
}

struct page *find_or_create_page(struct address_space *mapping,
					pgoff_t offset, gfp_t gfp_mask)
{
	return pagecache_get_page(mapping, offset,
					FGP_LOCK|FGP_ACCESSED|FGP_CREAT,
					gfp_mask);
}

int file_remove_suid(struct file *file)
{
	return file_remove_privs(file);
}

/*
  based on https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/diff/?id=9a219bc70b54523c8f14a3c957179854785ee97c
  do_sync_read can be replace by new_sync_read,but it didn`t export symbol since Linux kernel v4.1
  use the __vfs_read to replace new_sync_read
  the __vfs_read did not export symbol since Linux kernel v4.14, can use kernel_read to replace
  do_sync_read removed since Linux kernel v4.1
*/
ssize_t do_sync_read(struct file *filp, char __user *buf, size_t len, loff_t *ppos)
{
	return __vfs_read(filp, buf, len, ppos);
}

ssize_t do_sync_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
	return __vfs_write(filp, buf, len, ppos);
}

/*
struct buffer_head *__bread(struct block_device *bdev, sector_t block, unsigned size)
{
	return __bread_gfp(bdev, block, size, __GFP_MOVABLE);
}

struct buffer_head *__getblk(struct block_device *bdev,
					   sector_t block,
					   unsigned size)
{
	return __getblk_gfp(bdev, block, size, __GFP_MOVABLE);
}
*/

//注意：kernel 2.6.36中定义不一样（3.10.14同上）,处理方式为搁置
//void __lru_cache_add(struct page *page, enum lru_list lru)
void __lru_cache_add(struct page *page, enum lru_list lru)
{
	struct pagevec *pvec = &get_cpu_var(lru_add_pvec);

	page_cache_get(page);
	if (!pagevec_space(pvec))
		__pagevec_lru_add(pvec);
	pagevec_add(pvec, page);
	put_cpu_var(lru_add_pvec);

/*	kernel 2.6.36
	struct pagevec *pvec = &get_cpu_var(lru_add_pvecs)[lru];

	page_cache_get(page);
	if (!pagevec_add(pvec, page))
		____pagevec_lru_add(pvec, lru);
	put_cpu_var(lru_add_pvecs);
*/
}

//generic_file_direct_write:待完成
