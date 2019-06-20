#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/bio.h>

/*
  based on https://lore.kernel.org/patchwork/patch/589398/
  We can always fill up the bio now, no need to estimate the possible
  size based on queue parameters.
  bio_get_nr_vecs removed since Linux kernel v4.3 
*/
int bio_get_nr_vecs(struct block_device *bdev)
{
	return BIO_MAX_PAGES;
}
//EXPORT_SYMBOL(bio_get_nr_vecs);
