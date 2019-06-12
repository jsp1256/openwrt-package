/* rename some inline functions to resolve errors */
#define msecs_to_jiffies msecs_to_jiffies_orig
#define generic_write_sync generic_write_sync_orig
#define __blockdev_direct_IO __blockdev_direct_IO_orig

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include "config.h"
#include "ufsdapi.h"
#undef msecs_to_jiffies
#undef generic_write_sync
#undef __blockdev_direct_IO

#undef __copy_from_user
#undef __copy_to_user
#undef printk_ratelimit

static void fool(const char * name)
{
	printk("fatal error: fool function %s called\n", name);
	mdelay(1000);
	BUG();
	*(int*)0=0;
}

#define MAKE_FOOL(x) void x(void){fool(#x);}

/* MAKE_FOOL(); */

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
MAKE_FOOL(__bread);
MAKE_FOOL(bio_alloc);
MAKE_FOOL(malloc_sizes);
MAKE_FOOL(test_set_page_writeback);
MAKE_FOOL(generic_file_aio_read);
MAKE_FOOL(end_writeback);
MAKE_FOOL(generic_write_sync);
MAKE_FOOL(d_alloc_root);
MAKE_FOOL(file_remove_suid);
MAKE_FOOL(bio_get_nr_vecs);
MAKE_FOOL(balance_dirty_pages_ratelimited_nr);
MAKE_FOOL(__lru_cache_add);
MAKE_FOOL(msecs_to_jiffies);
MAKE_FOOL(do_sync_read);
MAKE_FOOL(kthread_create);
MAKE_FOOL(do_sync_write);
MAKE_FOOL(generic_file_splice_write);
MAKE_FOOL(writeback_inodes_sb_if_idle);
MAKE_FOOL(get_sb_bdev);
MAKE_FOOL(__blockdev_direct_IO);
MAKE_FOOL(generic_segment_checks);
MAKE_FOOL(__getblk);
