/*---------------------------------------------------------------------------
* /drivers/stm/lpm_i2c.c
* Copyright (C) 2011 STMicroelectronics Limited
* Contributor:Francesco Virlinzi <francesco.virlinzi@st.com>
* Author:Pooja Agarwal <pooja.agarwal@st.com>
* Author:Udit Kumar <udit-dlh.kumar@st.cm>
* May be copied or modified under the terms of the GNU General Public
* License.  See linux/COPYING for more information.
*----------------------------------------------------------------------------
*/

#include <linux/stm/lpm.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/errno.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/stm/platform.h>
#include "lpm_def.h"

#ifdef CONFIG_STM_LPM_DEBUG
#define lpm_debug printk
#else
#define lpm_debug(fmt,arg...);
#endif

struct stm_lpm_driver_data{
        struct i2c_adapter *I2C_Adapter;
        int gbltrans_id;
        struct mutex msg_protection_mutex;
};


static struct stm_lpm_driver_data *lpm_drv;

int stm_lpm_setup_ir(int num_of_sequence, int byte_count,int protocol, struct stm_lpm_ir_setting *byte_sequence){
        struct stlpm_internal_send_msg	send_msg={0};
        struct stm_lpm_message response;
     	char *msg;
        int err=0, msg_size;
        if(!num_of_sequence || !byte_count || !protocol)
                return -EINVAL;
        msg=kmalloc(num_of_sequence*4+3,GFP_KERNEL);
        if(!msg) return -ENOMEM;
        msg[0]=(protocol<<4);
        msg[1]=num_of_sequence;
        msg[2]=byte_count;

        memcpy(msg+3,&byte_sequence,num_of_sequence*4);
        msg_size=(num_of_sequence*4)+3;
        LPM_FILL_MSG(send_msg,STM_LPM_MSG_SET_IR,msg,msg_size,MSG_ID_AUTO,SBC_REPLY_YES);
        err=lpm_exchange_msg(&send_msg,&response); ;
        kfree(msg);
	    return err;
}
EXPORT_SYMBOL(stm_lpm_setup_ir);

int stm_lpm_get_wakeup_info(enum stm_lpm_wakeup_devices *wakeupdevice, int *validsize, int
datasize, int *data)
{
	int size,err = 0;
        char msg[3];
        struct stlpm_internal_send_msg send_msg={0};
        struct stm_lpm_message response={0};

        msg[0]=*wakeupdevice;
		/*Do not send more size we have buffer of 16 bytes to get response*/
		if(datasize>STM_LPM_MAX_MSG_DATA)
			datasize=STM_LPM_MAX_MSG_DATA;
        memcpy(&msg[1],&datasize, 2);
		
        LPM_FILL_MSG(send_msg,STM_LPM_MSG_GET_IRQ,msg,3,MSG_ID_AUTO,SBC_REPLY_YES);
        err=lpm_exchange_msg(&send_msg,&response); 
		memcpy((void*)&size, &response.msg_data[0], 2);
		//to get the actual size reset 15th bit(m)
		size &= M_BIT_MASK;

		memcpy((void*)data,(const void*)&response.msg_data[4],size);
		*validsize=size;
        
	//Check for m: more data available or not
	if(response.msg_data[1] & 0x80)	{
		// Returning positive error as need to read again	
		 return EAGAIN;
	}
        return err;
}
EXPORT_SYMBOL(stm_lpm_get_wakeup_info);

static int lpm_recv_respone_i2c (struct stm_lpm_message *msg){
        struct i2c_msg i2c_mssg = {.addr = ADDRESS_OF_EXT_MC, /*Device address of slave*/
                                   .flags = I2C_M_RD,
                                   .buf = (char *)msg,
                                   .len = 2};
        int err=1,i=0;
        lpm_debug("stm_lpm_recv_respone_i2c, size of msg 4 static address of ext device %d \n",i2c_mssg.addr);
        switch(msg->command_id){
                case STM_LPM_MSG_NOP:
                case STM_LPM_MSG_GET_IRQ :
                case STM_LPM_MSG_ERR :
                case STM_LPM_MSG_SET_TRIM :
                case STM_LPM_MSG_ENTER_PASSIVE:
                case STM_LPM_MSG_GEN_RESET:
                default:
                lpm_debug("stm_lpm_recv_respone_i2c, response not need \n");
                return err;

                /*Use default length of 2*/
                case STM_LPM_MSG_SET_TIMER :
                case STM_LPM_MSG_SET_IR:
                case STM_LPM_MSG_SET_FP :
                case STM_LPM_MSG_SET_WDT :
                case STM_LPM_MSG_SET_RTC :
                case STM_LPM_MSG_SET_WUD : break;


                case STM_LPM_MSG_VER : i2c_mssg.len=7; break;
                case STM_LPM_MSG_READ_RTC :i2c_mssg.len=8; break;
                case STM_LPM_MSG_GET_STATUS :
                case STM_LPM_MSG_GET_WUD :i2c_mssg.len=4; break;
        }
        err=i2c_transfer(lpm_drv->I2C_Adapter, &i2c_mssg, 1);
        lpm_debug("i2c_transfer response error is %d \n",err);
        if(err>=0) {
                lpm_debug("stm_lpm_recv_respone_i2c command id %d \n",msg->command_id);
                lpm_debug("stm_lpm_recv_respone_i2c transid id %d \n",msg->transaction_id);
                for(i=0;i<i2c_mssg.len-2;i++) lpm_debug("%X ",msg->msg_data[i]);
                lpm_debug("\n");
        }
        return err;
}

static int lpm_send_msg(char *msg, unsigned char msg_size,struct stm_lpm_message *response){
        int err=0;
                struct i2c_msg i2c_mssg = {.addr = ADDRESS_OF_EXT_MC /*Device address of slave*/,
                                           .flags = 0,
                                           .buf = (char *) msg,
                                           .len = msg_size+2};
                lpm_debug("stm_lpm_send_msg buf  %d and address is %d \n",*i2c_mssg.buf,i2c_mssg.addr);
                lpm_debug("stm_lpm_send_msg, size of msg %d \n",msg_size+2);

                err=i2c_transfer(lpm_drv->I2C_Adapter, &i2c_mssg, 1);
                lpm_debug("i2c_transfer send error is %d \n",err);
                if(err>=0){
                        /*Get the reply from external controller*/
                        err= lpm_recv_respone_i2c(response);
                }

        return err;
}


int lpm_exchange_msg(struct stlpm_internal_send_msg * send_msg, struct stm_lpm_message *response)
{
        char  *lpm_msg=NULL;
        struct stm_lpm_message msg={0};
        int count=0,err=0;

        lpm_debug("lpm_exchange_msg \n");
        if(send_msg->msg_size>14){
        /*We would enter into this condition very rarely*/
        lpm_msg = kmalloc(send_msg->msg_size+2,GFP_KERNEL);
            if(!lpm_msg) return -ENOMEM;
        }
        else lpm_msg=(char * )&msg;

        mutex_lock(&lpm_drv->msg_protection_mutex);

        lpm_msg[0]=send_msg->command_id;
        response->command_id=send_msg->command_id;
        lpm_msg[1]= lpm_drv->gbltrans_id++;
        if(send_msg->msg_size)
                memcpy(&lpm_msg[2],send_msg->msg,send_msg->msg_size);

        lpm_debug("Sending msg {%d, %d ",lpm_msg[0],lpm_msg[1]);
        for(count=0;count<send_msg->msg_size;count++)
                lpm_debug(" %d",lpm_msg[2+count]);
        lpm_debug(" } \n");

        err=lpm_send_msg(lpm_msg,send_msg->msg_size,response);

        lpm_debug("recd reply error %d {%d, %d ",err, response->command_id,
        response->transaction_id);
        for(count=0;count<14;count++)
                lpm_debug(" %d",response->msg_data[count]);
        lpm_debug("} \n");
        if(err){
                if(lpm_msg[1] == response->transaction_id){
                        /* Check for error in FWLPM response*/
                        if(response->command_id==STM_LPM_MSG_ERR){
                                /*just check error for command*/
                                printk("Error respone \n");
                                /* Get the actual error */
                                err=response->msg_data[3];
                        }
                        /* there is possibility we might get response for large msg*/
                }
                else {
                        lpm_debug("Received response tran ID %d instead of %d \n",response->transaction_id,lpm_msg[1]);
                        err=-EIO; /* invalid trans id*/
                }
        }
        else {
                lpm_debug("f/w is not reponding \n");
                err=-EAGAIN;
        }
        mutex_unlock(&lpm_drv->msg_protection_mutex);
        /*If we have allocated lpm_msg then free this message*/
        if(send_msg->msg_size>14)
            kfree(lpm_msg);
        return err;
}

static int stm_lpm_enter_passive_standby(void){
        struct stlpm_internal_send_msg send_msg;
        struct stm_lpm_message response;
        LPM_FILL_MSG(send_msg,STM_LPM_MSG_ENTER_PASSIVE,NULL,MSG_ZERO_SIZE,MSG_ID_AUTO,SBC_REPLY_NO);
        return(lpm_exchange_msg(&send_msg,&response));
}


#ifdef CONFIG_STM_LPM_RD_MONITOR
extern int stm_lpm_start_power_monitor(struct platform_device *pdev);
extern void stm_lpm_stop_power_monitor(struct platform_device *pdev);
#endif

static int __init stm_lpm_probe(struct platform_device *pdev){
        struct stm_plat_lpm_data *plat_data;
		int err=0;
        lpm_debug("stm lpm probe \n");
        lpm_drv=kzalloc(sizeof(struct stm_lpm_driver_data), GFP_KERNEL);
        if (unlikely(lpm_drv == NULL)) {
		    printk(KERN_ERR "%s: Request memory not done\n",__FUNCTION__);
    		return -ENOMEM;
		}
        plat_data = pdev->dev.platform_data;
        if (unlikely(plat_data == NULL)) {
		    dev_err(&pdev->dev, "No platform data\n");
    		err=-ENOENT;
			goto exit;
		}
        lpm_debug("i2c_id %d \n",plat_data->number_i2c);
        if(plat_data->number_i2c){
                lpm_drv->I2C_Adapter=i2c_get_adapter(plat_data->number_i2c);
                if(lpm_drv->I2C_Adapter==NULL){
                        printk(KERN_ERR "%s: Request i2c  7108 not done adapter %d not found \n",__FUNCTION__, plat_data->number_i2c);
                        err=-ENODEV;
						goto exit;
                }
                lpm_debug("stlpm i2c adapter found at %d i2c is %x \n",plat_data->number_i2c,(unsigned int)lpm_drv->I2C_Adapter);
        }

        /*mutex initialization*/
        mutex_init(&lpm_drv->msg_protection_mutex);

#ifdef CONFIG_STM_LPM_RD_MONITOR
          err=stm_lpm_start_power_monitor(pdev);
#endif
        return err;
exit:
	kfree(lpm_drv);
	return err;
}

static int stm_lpm_remove(struct platform_device *pdev){
        lpm_debug("stm_lpm_remove \n");
        i2c_put_adapter(lpm_drv->I2C_Adapter);
#ifdef CONFIG_STM_LPM_RD_MONITOR
        stm_lpm_stop_power_monitor(pdev);
#endif		
        kfree(lpm_drv);
        return 0;
}

static int stm_lpm_freeze(struct device *dev){
	lpm_debug("stlpm_suspend state \n");
        stm_lpm_enter_passive_standby();
        return 0;
}

static int stm_lpm_restore(struct device *dev){
	struct stm_plat_lpm_data *plat_data;
    lpm_debug("stm_lpm_restore \n");
	plat_data=dev->platform_data;
	plat_data->status_gpio=1;
        return 0;
}

static struct dev_pm_ops stm_lpm_pm_ops = {
        .freeze = stm_lpm_freeze,  /* on standby/memstandby */
        .restore = stm_lpm_restore,
};

static struct platform_driver stm_lpm_driver = {
        .driver.name = "stlpm",
        .driver.owner  = THIS_MODULE,
        .driver.pm     = &stm_lpm_pm_ops,
        .probe = stm_lpm_probe,
        .remove = stm_lpm_remove,
};



static int __init stm_lpm_init(void){
        int err=0;
        err=platform_driver_register(&stm_lpm_driver);
        printk("stm_lpm err %d \n", err);
        printk("STM_LPM driver registered\n");
        return err;
}

void __exit stm_lpm_exit(void){
        printk("STM_LPM driver removed \n");
        platform_driver_unregister(&stm_lpm_driver);
}

module_init(stm_lpm_init);
module_exit(stm_lpm_exit);

MODULE_AUTHOR("STMicroelectronics  <www.st.com>");
MODULE_DESCRIPTION("lpm device driver for STMicroelectronics devices");
MODULE_LICENSE("GPL");


