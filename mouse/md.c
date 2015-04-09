#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include "linux/interrupt.h"
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/ioport.h>
#include <linux/spinlock_types.h>

#define OURMOUSE_BASE 0x300
#define SPIN_LOCK_UNLOCKED      (spinlock_t) { 0, 0 }

static int mouse_users = 0;
static int mouse_dx = 0;
static int mouse_dy = 0;
static int mouse_event = 0;
static int mouse_buttons = 0;


static int open_mouse(struct inode *inode, struct file *file)
{
	unsigned int OURMOUSE_IRQ;

	if(mouse_users++)
		return 0;
	if(request_irq(5, OURMOUSE_IRQ, 0, "ourmouse", NULL))
	{
		mouse_users--;
		return -EBUSY;
	}
	mouse_dx = 0;
	mouse_dy = 0;
	mouse_event = 0;
	mouse_buttons = 0;
	return 0;
}

static int close_mouse(struct inode *inode, struct file *file)
{
	unsigned int OURMOUSE_IRQ;
	if(--mouse_users)
		return 0;
	free_irq(OURMOUSE_IRQ, NULL);
	return 0;
}

static ssize_t write_mouse(struct file *file, const char *buffer, size_t
	count, loff_t *ppos)
{
	return -EINVAL;
}


struct file_operations our_mouse_fops = {
	owner: THIS_MODULE,
/* Automatic usage management */
	// read: read_mouse,
/* You can read a mouse */
	write: write_mouse,
/* This won’t do a lot */
	// poll: poll_mouse,
/* Poll */
	open: open_mouse,
/* Called on open */
	release: close_mouse,
/* Called on close */
};


static struct miscdevice our_mouse = {
	MISC_DYNAMIC_MINOR, "ourmouse", &our_mouse_fops
};

static __init init_ourmouse(void)
{
	//printk(KERN_INFO "Hello, world \n");
	if (request_region(OURMOUSE_BASE, 3, "ourmouse") < 0) {
		printk(KERN_ERR "ourmouse: request_region failed.\n");
		return -ENODEV;
	}
	if (misc_register(&our_mouse) < 0) {
		printk(KERN_ERR "ourmouse: cannot register misc device.\n");
		release_region(OURMOUSE_BASE, 3);
		return -EBUSY;
	}
	return 0;
}



#ifdef MODULE

int init_module(void)
{
	if(init_ourmouse()<0)
		return -ENODEV;
	return 0;
}

void cleanup_module(void)
{
	misc_deregister(&our_mouse);
	release_region(OURMOUSE_BASE, 3);
}
#endif

static struct wait_queue *mouse_wait;
static spinlock_t mouse_lock = SPIN_LOCK_UNLOCKED;
static void ourmouse_interrupt(int irq, void *dev_id, struct pt_regs *regs)
{
	char delta_x;
	char delta_y;
	unsigned char new_buttons;
	delta_x = inb(OURMOUSE_BASE);
	delta_y = inb(OURMOUSE_BASE+1);
	new_buttons = inb(OURMOUSE_BASE+2);
	if(delta_x || delta_y || new_buttons != mouse_buttons)
	{
/* Something happened */
		spin_lock(&mouse_lock);
		mouse_event = 1;
		mouse_dx += delta_x;
		mouse_dy += delta_y;
		mouse_buttons = new_buttons;
		spin_unlock(&mouse_lock);
		//wake_up_interruptible(&mouse_wait);
	}
}

MODULE_LICENSE("GPL");
