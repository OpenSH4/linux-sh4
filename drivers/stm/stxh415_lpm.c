/*---------------------------------------------------------------------------
* /drivers/stm/stxh415_lpm.c
* Copyright (C) 2011 STMicroelectronics Limited
* Author:Francesco Virlinzi <francesco.virlinzi@st.com>
* Author:Pooja Agarwal <pooja.agarwal@st.com>
* Author:Udit Kumar <udit-dlh.kumar@st.cm>
* May be copied or modified under the terms of the GNU General Public
* License.  See linux/COPYING for more information.
*----------------------------------------------------------------------------
*/

#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/stm/platform.h>
#include <linux/stm/stih415.h>
/*To test on 7108 SOC*/
//#define SBC_ADDRESS   0xFDABF800
//#define SBC_SIZE      0x1000
//#define MAILBOX_IRQ_NUM   0x4C

/*Actual ORLY Settings*/
#define SBC_ADDRESS   0xFE400000
#define SBC_SIZE 0x100000
#define MAILBOX_IRQ_NUM   STIH415_IRQ(215)

static struct platform_device stm_lpm_device = {
        .name = "stlpm",
        .id = 0,
        .num_resources = 2,
        .resource = (struct resource[]){
                {       .start = SBC_ADDRESS,
                        .end   = SBC_ADDRESS+ SBC_SIZE,
                        .flags = IORESOURCE_MEM,
                },

                {       .start = MAILBOX_IRQ_NUM,/*mailbox interrupt*/
                        .end   = MAILBOX_IRQ_NUM,
                        .flags = IORESOURCE_IRQ
                }
        }
};

const char* lpm_get_cpu_type(void){ 
	return stm_soc();
}

static int __init stxh415_add_lpm(void)
{
	return platform_device_register(&stm_lpm_device);
}

module_init(stxh415_add_lpm);

	
	
