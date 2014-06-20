/*---------------------------------------------------------------------------
* /drivers/stm/lpm_gpio_monitor.c
* Copyright (C) 2011 STMicroelectronics Limited
* Author:Francesco Virlinzi <francesco.virlinzi@st.com>
* Author:Pooja Agarwal <pooja.agarwal@st.com>
* Author:Udit Kumar <udit-dlh.kumar@st.cm>
* May be copied or modified under the terms of the GNU General Public
* License.  See linux/COPYING for more information.
*----------------------------------------------------------------------------
*/
#ifdef CONFIG_STM_LPM_RD_MONITOR
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/ioport.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/pm.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/kthread.h>
#include <linux/hom.h>
#include <linux/stm/gpio.h>
#include <linux/stm/pms.h>
#include <linux/stm/lpm.h>
#include <linux/stm/platform.h>


static irqreturn_t monitor_gpio_handler(int irq, void *ptr){
		struct stm_plat_lpm_data *plat_data;
		plat_data=pdev->dev.platform_data;
        plat_data->status_gpio=0;
        return IRQ_HANDLED;
}


static ssize_t stm_lpm_show_powerkey(struct device *dev,
                struct device_attribute *attr, char *buf)
{
        int ret = 0;
        pr_info("\n power key pressed %s \n",wait_condition==0?"Yes":"No");
        ret = sprintf(buf, "%d \n", wait_condition);
        return ret;
}

static ssize_t stm_lpm_store_powerkey(struct device *dev,
                struct device_attribute *attr,
                const char *buf, size_t count)
{
        int value = 0;
        pr_info("\n");
        value =  simple_strtoul(buf, NULL, 4);
        pr_info(" application want to set power key to value = %d\n", value);
        return count;
}

static DEVICE_ATTR(powerkey, S_IRUGO | S_IWUSR, stm_lpm_show_powerkey,
                   stm_lpm_store_powerkey);

int __init stm_lpm_start_power_monitor(struct platform_device *pdev)
{
    int ret = 0;
    struct task_struct *th_monitor;
	struct stm_plat_lpm_data *plat_data;
	plat_data = pdev->dev.platform_data;
	plat_data->number_gpio=plat_data->number_gpio;
	printk("Platform data gpio no %d", plat_data->number_gpio);
    ret = gpio_request(plat_data->number_gpio, "monitor_gpio");

        if (ret < 0) {
                pr_err("ERROR: Request pin Not done!\n");
                return ret;
        }

        gpio_direction_input(plat_data->number_gpio);

        set_irq_type(gpio_to_irq(plat_data->number_gpio), IRQF_TRIGGER_FALLING);
        ret = request_irq(gpio_to_irq(plat_data->number_gpio), monitor_gpio_handler,
        IRQF_DISABLED, "monitor_irq", pdev);
        if (ret < 0) {
                gpio_free(plat_data->number_gpio);
				pr_err("ERROR: Request irq Not done!\n");
                return ret;
        }
        plat_data->status_gpio = 1;
        ret=device_create_file(&(pdev->dev), &dev_attr_powerkey);
		if (ret < 0) {
                pr_err("ERROR: Not able to  create files !\n");
				goto err;
                
        }
        return 0;
	err :
	free_irq(gpio_to_irq(plat_data->number_gpio),pdev);
	gpio_free(plat_data->number_gpio);
	return ret;
	
}
void stm_lpm_stop_power_monitor(struct platform_device *pdev)
{
	device_remove_file(&(pdev->dev), &dev_attr_powerkey);
	free_irq(gpio_to_irq(plat_data->number_gpio),pdev);
	gpio_free(plat_data->number_gpio);
}
#endif
