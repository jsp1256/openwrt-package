#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include "config.h"
#include "ufsdapi.h"

static void fool(const char * name)
{
	printk("fatal error: fool function %s called\n", name);
	mdelay(1000);
	BUG();
	*(int*)0=0;
}

#define MAKE_FOOL(x) void x(void){fool(#x);}

MAKE_FOOL(__bread);
MAKE_FOOL(__aeabi_unwind_cpp_pr1);
MAKE_FOOL(posix_acl_clone);
MAKE_FOOL(posix_acl_create_masq);
MAKE_FOOL(posix_acl_to_xattr);
MAKE_FOOL(generic_file_aio_read);
MAKE_FOOL(end_writeback);
MAKE_FOOL(generic_file_aio_write);
MAKE_FOOL(posix_acl_chmod_masq);
MAKE_FOOL(d_alloc_root);
MAKE_FOOL(posix_acl_equiv_mode);
MAKE_FOOL(_clear_bit_le);
MAKE_FOOL(posix_acl_permission);
MAKE_FOOL(posix_acl_valid);
MAKE_FOOL(posix_acl_from_xattr);
MAKE_FOOL(__bug);
MAKE_FOOL(generic_file_splice_write);
MAKE_FOOL(_test_and_set_bit_le);
MAKE_FOOL(writeback_inodes_sb_if_idle);
MAKE_FOOL(get_sb_bdev);
MAKE_FOOL(__blockdev_direct_IO);
MAKE_FOOL(block_sync_page);
MAKE_FOOL(__aeabi_unwind_cpp_pr0);
MAKE_FOOL(__getblk);
MAKE_FOOL(_set_bit_le);
MAKE_FOOL(malloc_sizes);
MAKE_FOOL(inode_setattr);
MAKE_FOOL(do_sync_write);
MAKE_FOOL(file_fsync);
MAKE_FOOL(create_proc_entry);
MAKE_FOOL(num_physpages);
MAKE_FOOL(do_sync_read);
MAKE_FOOL(bio_alloc);
MAKE_FOOL(block_prepare_write);
