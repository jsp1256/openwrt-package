#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include "config.h"
#include "ufsdapi.h"

#undef printk_ratelimit

static void fool(const char * name)
{
	printk("fatal error: fool function %s called\n", name);
	mdelay(1000);
	BUG();
	*(int*)0=0;
}

#define MAKE_FOOL(x) void x(void){fool(#x);}

MAKE_FOOL(__bread);
MAKE_FOOL(bio_alloc);
MAKE_FOOL(posix_acl_clone);
MAKE_FOOL(posix_acl_create_masq);
MAKE_FOOL(posix_acl_to_xattr);
MAKE_FOOL(test_set_page_writeback);
MAKE_FOOL(generic_file_aio_read);
MAKE_FOOL(posix_acl_chmod_masq);
MAKE_FOOL(d_alloc_root);
MAKE_FOOL(posix_acl_equiv_mode);
MAKE_FOOL(balance_dirty_pages_ratelimited_nr);
MAKE_FOOL(__lru_cache_add);
MAKE_FOOL(posix_acl_permission);
MAKE_FOOL(posix_acl_valid);
MAKE_FOOL(posix_acl_from_xattr);
MAKE_FOOL(generic_file_buffered_write);
MAKE_FOOL(generic_file_splice_write);
MAKE_FOOL(get_sb_bdev);
MAKE_FOOL(vmtruncate);
MAKE_FOOL(sync_page_range);
MAKE_FOOL(generic_segment_checks);
MAKE_FOOL(__getblk);

MAKE_FOOL(printk_ratelimit);
