#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/ctype.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>

#include <linux/string.h>

#define MODULE_NAME	"chris_leds"

static char *board = "v32g";

module_param(board, charp, S_IRUGO);

struct chris_leds_detail{
	char *name;
	int pin;
	int value;
};

struct chris_leds_t{
	int led_on;
	struct timer_list timer;
	struct chris_leds_detail led[4];
}chris_leds;

static int chris_leds_open(struct inode *inode, struct file *filp)
{
    int ret = 0;
    return ret;
}

static long chris_leds_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
    int ret = 0;

    return ret;
}

static int chris_leds_release(struct inode *inode, struct file *filp)
{
    int ret = 0;
    return ret;
}

struct file_operations chris_leds_ops = {
    .open       = chris_leds_open,
    .unlocked_ioctl      = chris_leds_ioctl,
    .release    = chris_leds_release,
};

struct miscdevice chris_leds_misc = {
    .name = "chris_leds_misc",
    .minor= MISC_DYNAMIC_MINOR,
    .fops = &chris_leds_ops,
};

static void chris_gpio_set_value(int *pin, int value)
{
	struct chris_leds_detail *led =container_of(pin, struct chris_leds_detail, pin);
	led->value = value;
	gpio_set_value(*pin, value);
}

static int chris_leds_gpio_init(struct chris_leds_t *chris_leds_p)
{
	int i, ret = -1;
	
	if(!strcmp(board, "qt210")){
		for(i = 0; i < 4; i++)
			chris_leds_p->led[i].pin = S5PV210_GPH0(i);
		printk("the board is qt210\n");
	}
	else if(!strcmp(board, "v32g")){
		for(i = 0; i < 4; i++)
			chris_leds_p->led[i].pin = S5PV210_GPJ3(i);
		printk("the board is v32g\n");
	}
	else{
		printk("module param error!!\n");
		return -1;
	}

	for(i = 0; i < 4; i++){
		chris_leds_p->led[i].name = kmalloc(10*sizeof(char), GFP_KERNEL);
		sprintf(chris_leds_p->led[i].name, "led%d", i);
		ret = gpio_request(chris_leds_p->led[i].pin, chris_leds_p->led[i].name);
		if(ret)
			goto leds_request_error;
		ret = s3c_gpio_cfgpin(chris_leds_p->led[i].pin, S3C_GPIO_OUTPUT);
		if(ret)
			goto leds_cfgpin_error;

		chris_gpio_set_value(&chris_leds_p->led[i].pin, 1);
	}

	return ret;
leds_cfgpin_error:

leds_request_error:
	for(i = 0; i < 4; i++)
		gpio_free(chris_leds_p->led[i].pin);
	
	return ret;
}

static void chris_leds_gpio_un_init(struct chris_leds_t *chris_leds_p)
{
	int i;
	printk("chris_leds_gpio_un_init Start...\n");
	
	for(i = 0; i < 4; i++){
		chris_gpio_set_value(&chris_leds_p->led[i].pin, 0);
		gpio_free(chris_leds_p->led[i].pin);
	}
	printk("chris_leds_gpio_un_init End...\n");
}

static ssize_t chris_leds_1_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	int ret = -1;
	struct chris_leds_t *chris_leds_p = &chris_leds;

	printk("chris_leds_1_show...\n");
	
	ret = sprintf(buf, "%d\n", chris_leds_p->led[1].value);
	return ret;
}

static ssize_t chris_leds_1_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	int ret = -1;
	char *after;
	int state = (int)simple_strtoul(buf, &after, 10);
	size_t count = after - buf;
	struct chris_leds_t *chris_leds_p = &chris_leds;

	printk("chris_leds_1_store...\n");
	
	if (isspace(*after))
		count++;

	if (count == size) {
		if (chris_leds_p->led[1].value != state)
			chris_gpio_set_value(&chris_leds_p->led[1].pin, state);
		ret = count;
	}

	return ret;
}

/*
#define DEVICE_ATTR(_name, _mode, _show, _store) \
struct device_attribute dev_attr_##_name = __ATTR(_name, _mode, _show, _store)
*/
static DEVICE_ATTR(chris_leds_1, 0644, chris_leds_1_show, chris_leds_1_store);

static void chris_leds_timer_function(unsigned long data)
{
	int i;
	struct chris_leds_t *chris_leds_p = (struct chris_leds_t *)data;

	for(i = 0; i < 4; i++)
		chris_gpio_set_value(&chris_leds_p->led[i].pin, 0);
	if(chris_leds_p->led_on == 4)
		chris_leds_p->led_on = 0;
	chris_gpio_set_value(&chris_leds_p->led[chris_leds_p->led_on].pin, 1);
	chris_leds_p->led_on++;
	mod_timer(&chris_leds_p->timer, jiffies + msecs_to_jiffies(100));
	//printk("chris_leds_timer_function...\n");
}

static int chris_leds_probe(struct platform_device *dev)
{
	int ret = -1;
	struct chris_leds_t *chris_leds_p = &chris_leds;
		
	printk("probe Start...\n");
	ret = chris_leds_gpio_init(chris_leds_p);
	if(ret){
		printk("chris_leds_gpio_init error\n");
		goto chris_leds_gpio_init_error;
	}

	init_timer(&chris_leds_p->timer);
	chris_leds_p->timer.function = chris_leds_timer_function;
	chris_leds_p->timer.data = (unsigned long)chris_leds_p;
	chris_leds_p->led_on = 0;
	mod_timer(&chris_leds_p->timer, jiffies + 1);
	
	ret = misc_register(&chris_leds_misc);
	if(ret){
		printk("misc_register error\n");
		goto misc_error;
	}
	
/*
static DEVICE_ATTR(offset_z, S_IRUGO|S_IWUSR|S_IWGRP,
		bma2x2_offset_z_show,
		bma2x2_offset_z_store);

static struct attribute *bma2x2_attributes[] = {
	&dev_attr_offset_z.attr,
	NULL
};

static struct attribute_group bma2x2_attribute_group = {
	.attrs = bma2x2_attributes
};

	err = sysfs_create_group(&data->input->dev.kobj,
			&bma2x2_attribute_group);
*/

	ret = device_create_file(&dev->dev, &dev_attr_chris_leds_1);

	printk("probe End...\n");
	return ret;

misc_error:

chris_leds_gpio_init_error:
	chris_leds_gpio_un_init(chris_leds_p);
	return ret;
}

static int chris_leds_remove(struct platform_device *dev)
{
	int ret = 0;
	struct chris_leds_t *chris_leds_p = &chris_leds;

	device_remove_file(&dev->dev, &dev_attr_chris_leds_1);
	ret = misc_deregister(&chris_leds_misc);
	del_timer_sync(&chris_leds_p->timer);
	chris_leds_gpio_un_init(chris_leds_p);
	return ret;
}

static void chris_leds_pdev_release(struct device *dev)
{

}

struct platform_device chris_leds_pdev= {
	.name = MODULE_NAME,
	.dev = {
		.release = chris_leds_pdev_release,
	},
};

struct platform_driver chris_leds_pdrv= {
	.probe = chris_leds_probe,
	.remove  = chris_leds_remove,
	.driver = {
		.name = MODULE_NAME,
	},
};

static int __init chris_led_init(void)
{
	int ret;

	printk("chris_led_init Start...\n");
	ret = platform_device_register(&chris_leds_pdev);
	if(ret){
		printk("platform_device_register error\n");
		goto ERROR_1;
	}
	
	ret = platform_driver_register(&chris_leds_pdrv);
	if(ret){
		printk("platform_driver_register error\n");
		goto ERROR_2;
	}
	
	printk("chris_led_init End...\n");
	return ret;

ERROR_2:
	platform_device_unregister(&chris_leds_pdev);
	
ERROR_1:

	return ret;
}

static void __exit chris_led_exit(void)
{
	printk("chris_led_exit Start...\n");
	platform_driver_unregister(&chris_leds_pdrv);
	platform_device_unregister(&chris_leds_pdev);
	printk("chris_led_exit End...\n");
}

module_init(chris_led_init);
module_exit(chris_led_exit);

MODULE_AUTHOR("ChrisChen");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple gpio leds driver");
