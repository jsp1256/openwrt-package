/* rename some inline functions to resolve errors */
#define msecs_to_jiffies msecs_to_jiffies_orig
#define generic_write_sync generic_write_sync_orig
#define __blockdev_direct_IO __blockdev_direct_IO_orig

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
#include "config.h"
#include "ufsdapi.h"
#undef msecs_to_jiffies
#undef generic_write_sync
#undef __blockdev_direct_IO

#undef __copy_from_user
#undef __copy_to_user
#undef printk_ratelimit

static DEFINE_PER_CPU(struct pagevec, lru_add_pvec);

/*声明的外部内核函数符号*/
extern struct buffer_head *__getblk_gfp(struct block_device *bdev,
				sector_t block, unsigned size, gfp_t gfp);
extern struct buffer_head *__bread_gfp(struct block_device *,
				sector_t block, unsigned size, gfp_t gfp);
extern struct bio *bio_alloc_bioset(gfp_t, unsigned int, struct bio_set *);
extern int __test_set_page_writeback(struct page *page, bool keep_write);
extern struct bio_set *fs_bio_set;
extern void __pagevec_lru_add(struct pagevec *pvec);
extern int vfs_fsync_range(struct file *file, loff_t start, loff_t end,
			   int datasync);
extern void clear_inode(struct inode *inode);
extern ssize_t generic_file_read_iter(struct kiocb *, struct iov_iter *);
extern struct task_struct *kthread_create_on_node(int (*threadfn)(void *data),
					   void *data,
					   int node,
					   const char namefmt[], ...);
extern struct dentry *d_make_root(struct inode *root_inode);
extern int file_remove_privs(struct file *file);
extern void balance_dirty_pages_ratelimited(struct address_space *mapping);
extern ssize_t __vfs_read(struct file *, char __user *, size_t, loff_t *);
extern ssize_t generic_perform_write(struct file *, struct iov_iter *, loff_t);
extern ssize_t __vfs_write(struct file *, const char __user *, size_t, loff_t *);
extern ssize_t iter_file_splice_write(struct pipe_inode_info *,
		struct file *, loff_t *, size_t, unsigned int);
extern bool try_to_writeback_inodes_sb(struct super_block *, enum wb_reason reason);
extern struct dentry *mount_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int));

//declare for malloc_sizes
//该方案可能存在异常，建议适配最新的方案,不过kmalloc_size似乎不大好写
struct cache_sizes {
	size_t		 	cs_size;
	struct kmem_cache	*cs_cachep;
#ifdef CONFIG_ZONE_DMA
	struct kmem_cache	*cs_dmacachep;
#endif
};

struct cache_sizes malloc_sizes[] = {
#define CACHE(x) { .cs_size = (x) },
#include "kmalloc_sizes3.7.h"
	CACHE(ULONG_MAX)
#undef CACHE
};
//EXPORT_SYMBOL(malloc_sizes);

//替代函数
struct buffer_head *__getblk(struct block_device *bdev,
					   sector_t block,
					   unsigned size)
{
	return __getblk_gfp(bdev, block, size, __GFP_MOVABLE);
}
//EXPORT_SYMBOL(__getblk);

struct buffer_head *__bread(struct block_device *bdev, sector_t block, unsigned size)
{
	return __bread_gfp(bdev, block, size, __GFP_MOVABLE);
}
//EXPORT_SYMBOL(__bread);

struct bio *bio_alloc(gfp_t gfp_mask, unsigned int nr_iovecs)
{
	return bio_alloc_bioset(gfp_mask, nr_iovecs, fs_bio_set);
}
//EXPORT_SYMBOL(bio_alloc);

//注意：kernel 2.6.36中定义不一样
//void __lru_cache_add(struct page *page, enum lru_list lru)
void __lru_cache_add(struct page *page)
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
//EXPORT_SYMBOL(__lru_cache_add);

#undef test_set_page_writeback
int test_set_page_writeback(struct page *page)
{			
	return __test_set_page_writeback(page, false);
}
//EXPORT_SYMBOL(test_set_page_writeback);

int generic_write_sync(struct file *file, loff_t pos, loff_t count)
{
	if (!(file->f_flags & O_DSYNC) && !IS_SYNC(file->f_mapping->host))
		return 0;
	return vfs_fsync_range(file, pos, pos + count - 1,
			       (file->f_flags & __O_SYNC) ? 0 : 1);
}
//EXPORT_SYMBOL(generic_write_sync);

void end_writeback(struct inode *inode)
{
	clear_inode(inode);
}
//EXPORT_SYMBOL(end_writeback);

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
	
	retval = generic_segment_checks(&iov, &nr_segs, &count, VERIFY_WRITE);
	if (retval)
		return retval;
	iov_iter_init(&i, READ, iov, nr_segs, count);
	return generic_file_read_iter(iocb, &i);
}
//EXPORT_SYMBOL(generic_file_aio_read);

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
//EXPORT_SYMBOL(generic_file_buffered_write);

struct task_struct *kthread_create(int (*threadfn)(void *data),
					   void *data,
					   const char namefmt[], ...)
{
	//因为无法直接传递可变参数，不得不做阉割处理
	return kthread_create_on_node(threadfn, data, NUMA_NO_NODE, namefmt);
}
//EXPORT_SYMBOL(kthread_create);

struct dentry * d_alloc_root(struct inode * root_inode)
{
	return d_make_root(root_inode);
}
//EXPORT_SYMBOL(d_alloc_root);

int file_remove_suid(struct file *file)
{
	return file_remove_privs(file);
}
//EXPORT_SYMBOL(file_remove_suid);

/*
  based on https://lkml.org/lkml/2012/8/18/39
  There is no reason nr_pages_dirtied is argument anymore.
  because nr_pages_dirtied value from caller is not used in
  balance_dirty_pages_ratelimited_nr function.
  balance_dirty_pages_ratelimited_nr removed since Linux kernel v3.8
*/
void balance_dirty_pages_ratelimited_nr(struct address_space *mapping,
					unsigned long nr_pages_dirtied)
{
	balance_dirty_pages_ratelimited(mapping);
}
//EXPORT_SYMBOL(balance_dirty_pages_ratelimited_nr);

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
//EXPORT_SYMBOL(do_sync_read);

ssize_t do_sync_write(struct file *filp, const char __user *buf, size_t len, loff_t *ppos)
{
	return __vfs_write(filp, buf, len, ppos);
}

//EXPORT_SYMBOL(do_sync_write);

/*
  based on https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/diff/?id=8d0207652cbe27d1f962050737848e5ad4671958
  it will be replaced by iter_file_splice_write
  generic_file_splice_write removed since Linux kernel v3.16
*/
ssize_t generic_file_splice_write(struct pipe_inode_info *pipe, struct file *out, loff_t *ppos, size_t len, unsigned int flags)
{
	return iter_file_splice_write(pipe, out, ppos, len, flags);
}
//EXPORT_SYMBOL(generic_file_splice_write);

/*
  based on https://lkml.org/lkml/2011/12/8/267
  try to use try_to_writeback_inodes_sb_nr to replace
  assume writeback_inodes_sb_if_idle V1(one parameter)
*/
int writeback_inodes_sb_if_idle(struct super_block *sb)
{
	return (int)try_to_writeback_inodes_sb(sb, WB_REASON_FREE_MORE_MEM);
}
//EXPORT_SYMBOL(writeback_inodes_sb_if_idle);

/*
  This code form Linux kernel v2.6.38
  get_sb_bdev removed since Linux kernel v3.0 
*/
int get_sb_bdev(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data,
	int (*fill_super)(struct super_block *, void *, int),
	struct vfsmount *mnt)
{
	struct dentry *root;

	root = mount_bdev(fs_type, flags, dev_name, data, fill_super);
	if (IS_ERR(root))
		return PTR_ERR(root);
	mnt->mnt_root = root;
	mnt->mnt_sb = root->d_sb;
	return 0;
}
//EXPORT_SYMBOL(get_sb_bdev);

/*
static void fool(const char * name)
{
	printk("fatal error: fool function %s called\n", name);
	mdelay(1000);
	BUG();
	*(int*)0=0;
}

#define MAKE_FOOL(x) void x(void){fool(#x);}

//MAKE_FOOL(); 

MAKE_FOOL(__copy_from_user);
MAKE_FOOL(__copy_to_user);
MAKE_FOOL(posix_acl_to_xattr);
MAKE_FOOL(jnl_op);
MAKE_FOOL(posix_acl_chmod);
MAKE_FOOL(posix_acl_equiv_mode);
MAKE_FOOL(posix_acl_create);
MAKE_FOOL(posix_acl_valid);
MAKE_FOOL(posix_acl_from_xattr);

MAKE_FOOL(posix_acl_clone);
MAKE_FOOL(posix_acl_create_masq);
MAKE_FOOL(printk_ratelimit);
MAKE_FOOL(posix_acl_chmod_masq);
MAKE_FOOL(posix_acl_permission);
MAKE_FOOL(vmtruncate);
MAKE_FOOL(sync_page_range);

MAKE_FOOL(generic_file_buffered_write);
MAKE_FOOL(__bread);							OK
MAKE_FOOL(bio_alloc);						OK
MAKE_FOOL(malloc_sizes);					OK
MAKE_FOOL(test_set_page_writeback);			OK
MAKE_FOOL(generic_file_aio_read);			OK
MAKE_FOOL(end_writeback);					OK
MAKE_FOOL(generic_write_sync);				OK
MAKE_FOOL(d_alloc_root);					OK
MAKE_FOOL(file_remove_suid);				OK
MAKE_FOOL(bio_get_nr_vecs);					OK
MAKE_FOOL(balance_dirty_pages_ratelimited_nr);	OK
MAKE_FOOL(__lru_cache_add);					OK
MAKE_FOOL(msecs_to_jiffies);				OK...in other file
MAKE_FOOL(do_sync_read);					OK
MAKE_FOOL(kthread_create);					OK
MAKE_FOOL(do_sync_write);					OK
MAKE_FOOL(generic_file_splice_write);		OK
MAKE_FOOL(writeback_inodes_sb_if_idle);		OK
MAKE_FOOL(get_sb_bdev);						
MAKE_FOOL(__blockdev_direct_IO);
MAKE_FOOL(generic_segment_checks);			OK...implicited
MAKE_FOOL(__getblk);						OK
*/
