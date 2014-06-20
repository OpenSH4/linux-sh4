#include <linux/init.h>
#include <linux/module.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/mtd/mtd.h>
#include <asm/processor.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/ctype.h>

#include "flash_rwmode_set.h"

#define DEV_MAJOR_MTD_RWMODE_SET	0
static int rwmode_set_major = DEV_MAJOR_MTD_RWMODE_SET;
#define RWMODE_SET_DEVS_NUM 1

struct rwmode_set_dev
{
	struct cdev cdev;
};

struct rwmode_set_dev *devp;

struct mtd_info *get_mtd_info(struct inode *inode, struct file *file)
{
	int minor = iminor(inode);
	int devnum = minor >> 1;
	struct mtd_info *mtd;

	if (devnum >= MAX_MTD_DEVICES)
		return ERR_PTR(-ENODEV);

	mtd = get_mtd_device(NULL, devnum);
	if (IS_ERR(mtd))
		return mtd ;

	if (MTD_ABSENT == mtd->type) {
		put_mtd_device(mtd);
		return ERR_PTR(-ENODEV);
	}

	return mtd;
}

void put_mtd_info(struct mtd_info *mtd)
{
	/*
	if (mtd->sync)
	 	mtd->sync(mtd);
	*/

	put_mtd_device(mtd);
}

int flash_setrdonly(const char *filename)
{	
	struct mtd_info *mtd;
	struct file *f = filp_open(filename, O_RDONLY, 0); /* You can't open it RW if it's not a writeable device */
	if (IS_ERR(f))
		return PTR_ERR(f);

	mtd = get_mtd_info(f->f_dentry->d_inode, f);
	if (IS_ERR(mtd))
		return PTR_ERR(mtd);

	mtd->flags &= ~MTD_WRITEABLE; /*force read only*/
	put_mtd_info(mtd);
	filp_close(f, NULL);
	return 0;
}

int flash_setrdwr(const char *filename)
{	
	struct mtd_info *mtd;
	struct file *f = filp_open(filename, O_RDONLY, 0);/* You can't open it RW if it's not a writeable device */
	if (IS_ERR(f))
		return PTR_ERR(f);

	mtd = get_mtd_info(f->f_dentry->d_inode, f);
	if (IS_ERR(mtd))
		return PTR_ERR(mtd);
	mtd->flags |= MTD_WRITEABLE;
	put_mtd_info(mtd);
	filp_close(f, NULL);
	return 0;
}

static int rwmode_open(struct inode *inode,struct file *filp)
{
	filp->private_data = devp; 
	return 0;
}

static int rwmode_release(struct inode *inode,struct file *filp)
{
	return 0;
}

static int rwmode_ioctl(struct inode *inode,struct file *file,unsigned int 
							cmd,unsigned long arg)
{
	int ret = 0;
	char __user *argp = (char __user*)arg;
	size_t len = strlen_user(argp);
	char *fname;

	if (len <=0) {
		return -ENOENT;
	}
	printk(KERN_ALERT "len:%d\n", len);

	fname = kmalloc(sizeof(char)*len, GFP_KERNEL);
	if (NULL == fname)
		return -ENOMEM;

	if (copy_from_user(fname, argp, len)) {
		ret = -EFAULT;
		goto out;
	}

 	if (strncmp("/dev/mtd", fname, strlen("/dev/mtd"))) {
		ret = -EINVAL;
		goto out;
	}

	switch (cmd) {
		case MEMSETRDONLY:
		{
			ret = flash_setrdonly(fname);
			break;
		}

		case MEMSETRDWR:
		{
			ret = flash_setrdwr(fname);
			break;
		}

		default:
			ret = -ENOTTY;
	}
out:
	kfree(fname);
	return ret;
}

static const struct file_operations rwmode_fops=
{			
	.owner	= THIS_MODULE,
	.ioctl	= rwmode_ioctl,
	.open	= rwmode_open,
	.release = rwmode_release,
};

static int flash_rwmode_set_init(void)
{
	int ret = -1;
	dev_t dev =MKDEV(rwmode_set_major, 0);
	if (rwmode_set_major) {
		ret = register_chrdev_region(dev, RWMODE_SET_DEVS_NUM, "mtd_rwmode_set"); 
	} 
	else {
		ret = alloc_chrdev_region(&dev, 0, RWMODE_SET_DEVS_NUM, "mtd_rwmode_set");
		rwmode_set_major = MAJOR(dev);
	}

	if (ret < 0) {
		printk(KERN_NOTICE "MTD_RWMODE_SET: can't get major %d", rwmode_set_major);
		return ret;
	} 

	devp = kzalloc(sizeof(struct rwmode_set_dev), GFP_KERNEL);
	if (NULL == devp) {
		ret = -ENOMEM;
		goto fail_kmalloc;
	}

	cdev_init(&devp->cdev, &rwmode_fops);
	devp->cdev.owner = THIS_MODULE;
	devp->cdev.ops = &rwmode_fops;
	ret = cdev_add(&devp->cdev, dev, 1);
	if (ret < 0) {
		printk(KERN_NOTICE "Error %d adding rwmode_cdev", ret);	
		goto fail_cdev_add;
	}

	return ret;

fail_cdev_add:
	cdev_del(&devp->cdev);
	kfree(devp);

fail_kmalloc:
	unregister_chrdev_region(MKDEV(rwmode_set_major, 0), RWMODE_SET_DEVS_NUM);
	return ret;
}


static void flash_rwmode_set_exit(void)
{
	cdev_del(&devp->cdev);
    kfree(devp);
    unregister_chrdev_region(MKDEV(rwmode_set_major, 0), RWMODE_SET_DEVS_NUM);
}


module_init(flash_rwmode_set_init);
module_exit(flash_rwmode_set_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("zeng ming <ming.zeng@fulan.vicp.cc>");

