#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include <linux/io.h>
#include <linux/timer.h>


MODULE_AUTHOR("Takuma Okaue");
MODULE_DESCRIPTION("driver for led control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;
static struct cdev cdv;
static volatile u32 *gpio_base = NULL;
static struct class *cls = NULL;
static const u32 lednum[] = { 0x00,0x01,0x03,0x07,0x0f,0x1f};

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
	    int size = 0;
	     char sushi[] = {0xf0,0x9f,0x8d,0xa3,0x0a}; //寿司の絵文字のバイナリ   
	     if(copy_to_user(buf+size,(const char *)sushi, sizeof(sushi))){
	
		     printk( KERN_INFO "sushi : copy_to_user failed\n" );
		     return -EFAULT;
					        }
		           size += sizeof(sushi);
			       return size;
}

static ssize_t led_write(struct file* filp, const char* buf,size_t count,loff_t* pos){
	char c;
	int number;
	if(copy_from_user(&c,buf,sizeof(char))){
	return -EFAULT;
	}
	
	if('0'<=c&&c<='5'){

                gpio_base[10] = lednum[5] << 20;
		number = c - '0';
	        gpio_base[7] = lednum[number] << 20;
	}
	 else if(c == 'q'){
		 gpio_base[10] = lednum[5] << 20;
		     
        }

	       
return 1;

}




static struct file_operations led_fops = {
	.owner = THIS_MODULE,
	.write = led_write,
	.read = sushi_read	

};


static int __init init_mod(void)
{

	int retval;
	gpio_base = ioremap_nocache(0x3f200000, 0xA0);

	gpio_base[2] = 0xffff9249;
	retval = alloc_chrdev_region(&dev,0,1,"myled");
	if(retval < 0){
		printk(KERN_ERR "alloc_chrdev_region failed.\n");
        }
	printk(KERN_INFO "%s is unload.\n",__FILE__);
	printk(KERN_INFO "%s isloaded. major:%d\n",__FILE__,MAJOR(dev));

	cdev_init(&cdv, &led_fops);
	retval = cdev_add(&cdv, dev,1);
	if(retval <0){
		printk(KERN_ERR "cdev_add failed. major:%d,minor:%d",MAJOR(dev),MINOR(dev));
		return retval;
	}
	cls = class_create(THIS_MODULE,"myled");
	if(IS_ERR(cls)){
		printk(KERN_ERR "class_create failed.");
		return PTR_ERR(cls);
	}
	device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
	return 0;
}
static void __exit cleanup_mod(void)
{
	cdev_del(&cdv);
	device_destroy(cls,dev);
	class_destroy(cls);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "%s is unloaded.\n",__FILE__);
}

module_init(init_mod);
module_exit(cleanup_mod);
