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

/*Actual Lille Settings*/
#define SBC_ADDRESS   0xFE400000
#define SBC_SIZE 0xB4400
#define MAILBOX_IRQ_NUM   (176+64)

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
	return get_cpu_subtype(&current_cpu_data);	
}

static int __init stxh205_add_lpm(void)
{
	return platform_device_register(&stm_lpm_device);
}

module_init(stxh205_add_lpm);

