/*$Source: D:/STB_BASE_HAL/vfd.fulan.7167/test/input_test.c,v $*/
/*****************************文件头部注释*************************************/
//
//			Copyright (C), 2008-2013, AV Frontier Tech. Co., Ltd.
//
//
// 文 件 名：	$RCSfile: input_test.c,v $
//
// 创 建 者：	D02SH
//
// 创建时间：	2008.11.19
//
// 最后更新：	$Date: 2011/07/07 10:47:58 $
//
//				$Author: d27lwj $
//
//				$Revision: 1.1.1.1 $
//
//				$State: Exp $
//
// 文件描述：	红外遥控INPUT设置测试
//
/******************************************************************************/

/********************************  文件包含************************************/
#include <stdio.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <fcntl.h>
#include "panel.h"

#include <linux/input.h>


//#include "vfd_ext.h"

/********************************  常量定义************************************/
#define TS_DEV "/dev/input/event0"  //需要根据情况改变
#define TS_VFD "/dev/vfd"
static int ts_fd = -1;
static int ts_vfd_fd = -1;

/********************************  数据结构************************************/

/********************************  宏 定 义************************************/

/********************************  变量定义************************************/

/********************************  变量引用************************************/

/********************************  函数声明************************************/

/********************************  函数定义************************************/
 void delay(int delaytime)
 {    
     int i ,j ;
     for(i=0 ;i <delaytime; i++)
     {
         for(j=5000;j>0;j--);
     }
 };

 static int init_device(void)
{
    if ((ts_fd = open ( TS_DEV, O_RDONLY)) < 0)
    {
        printf("Error opening %s device \n",TS_DEV);
        return -1;
    }
    if ((ts_vfd_fd = open ( TS_VFD, O_RDWR)) < 0)
    {
        printf("Error opening %s device \n",TS_VFD);
        return -1;
    }
    return 0;
}

int main(void)
{
    int i;
    struct input_event data;

    if (init_device()<0)
        return -1;
    char show_str[] = "shihai";
    YWVFD_Format_T show_log;
    YWVFD_Time_T show_time;
    show_time.hour = 12;
    show_time.mint = 34; 
    show_log.LogNum = CAB;
    show_log.LogSta = LOG_ON;
    for(;;)
    {
        /*ioctl(ts_vfd_fd ,VFD_ShowLog,&show_log);
        show_log.LogNum--;
        if(show_log.LogNum < PLAY_FASTBACKWARD)
        {
            show_log.LogNum = CAB;
            show_log.LogSta++;
            show_log.LogSta = show_log.LogSta % 2; 
        }
        ioctl(ts_vfd_fd ,VFD_ShowTime,&show_time);*/////
        ioctl(ts_vfd_fd ,VFD_ShowStr,&show_str);
        //read(ts_fd,&data,sizeof(data));     
        //printf("Lirce Remote event type %d code= %d, value = %d \n",data.type, data.code,data.value);
        delay(1000);delay(1000);delay(1000); 
       // ioctl(ts_vfd_fd,VFD_ClearTime,0);//////
        delay(1000);delay(1000);delay(1000); 
    }
    return 0;
}

/* EOF------------------------------------------------------------------------*/

/* BOL-------------------------------------------------------------------*/
//$Log: input_test.c,v $
//Revision 1.1.1.1  2011/07/07 10:47:58  d27lwj
//no message
//
//Revision 1.1  2010/07/14 04:56:35  d27lwj
//升级NAND FLASH时面板驱动程序
//
//Revision 1.1.1.1  2009/05/31 08:01:47  d02sh
//no message
//
//Revision 1.1.1.1  2009/04/03 07:19:14  d22cj
//sparkdrv_stsdk版首次导入，与sparkdrv_0_4_00对应
//
//Revision 1.2  2008/12/02 05:47:01  d02sh
//增加面板ＶＦＤ相应的测试
//
//Revision 1.4  2008/12/01 11:34:31  d02sh
//修改遥控按键的ＩＮＰＵＴ响应方式
//ＶＦＤ面板显示出现的ＢＵＧ
//
//Revision 1.3  2008/11/21 12:46:36  d02sh
//新增IOCTL[用于面板控制]
//
//Revision 1.2  2008/11/21 12:10:11  d02sh
//面板可用[功能待完善]
//
//Revision 1.1  2008/11/19 08:42:41  d02sh
//红外遥控[可用]同时包含面板[正在调试]
//
//Revision 1.2  2008/11/19 08:25:49  d02sh
//遥控器测试代码
//
/* EOL-------------------------------------------------------------------*/

