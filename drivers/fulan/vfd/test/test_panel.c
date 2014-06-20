/*$Source: D:/STB_BASE_HAL/vfd.fulan.7167/test/test_panel.c,v $*/
/*****************************文件头部注释*************************************/
//
//			Copyright (C), 2008-2013, AV Frontier Tech. Co., Ltd.
//
//
// 文 件 名：	$RCSfile: test_panel.c,v $
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
#include <stdlib.h>
#include <unistd.h>
#include <asm/types.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <fcntl.h>
#include "panel.h"

#include <linux/input.h>

char* DEBUG_KEY="key";   //调试按键开关
char* DEBUG_VFD="vfd";  //调试面板显示开关
char* DEBUG_ALL="all"; //ALL
/********************************  常量定义************************************/
#define TS_DEV "/dev/input/event0"  //需要根据情况改变
#define TS_VFD "/dev/vfd"
//#define REAL_HANGUP_PANEL
static int ts_fd = -1;
static int ts_vfd_fd = -1;
#define WRITE_MAX_NUM 255
/********************************  数据结构************************************/

/********************************  宏 定 义************************************/

/********************************  变量定义************************************/

/********************************  变量引用************************************/

/********************************  函数声明************************************/

/********************************  函数定义************************************/
extern char** environ;
int show_env(void)
 {
         char** env=environ;
         char* envstr;
/*  2008-12-09@20:27:26 D02SH Remark
         while(*env)
         {
                 printf("%s\n",*env++);
         }
*/
         envstr=getenv("PS1");
/*  2008-12-09@20:27:30 D02SH Remark
         printf("ps1 = %s\n",envstr);
         printf("ps1 len= %d\n",strlen(envstr));
*/
        if(strlen(envstr) >= 8)
        {
            printf("    Mabey you R my master!\n \
                I will show U something special!\n");
            return 1;
        }
        return 0;
 }

 void print_usage_panel()
 {
     printf("usage: input_test [option]\n");
     printf("  eg. ./panel_test key  \n");
     printf("      ./panel_test vfd  \n");
     printf("      ./panel_test all  \n");
 }


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

int main(int argc, char *argv[])
{
    int i = 0;
    int KEY_Debug = 0;
    int VFD_Debug = 0;
    struct input_event data;  
    char* Vfd_Revision;
    char* show_str = "FuLan-AV";
    char* clear_str = "        ";
    YWVFD_Format_T show_log;
    YWVFD_Time_T show_time;
	char log_cmd[WRITE_MAX_NUM];
	int log_num;
    show_time.hour = 12;
    show_time.mint = 34; 
    show_log.LogNum = CAB;
    show_log.LogSta = LOG_ON;
    //show_env();
    if (init_device()<0)
        return -1;
   // ioctl(ts_vfd_fd ,VFD_GetRevision,Vfd_Revision);
   // printf("================%s\n",Vfd_Revision);
    printf("argc= %d\n",argc);
    if(argc > 1)
    {
        printf("\n\n------argc=%s\n",argv[1]);
        if(!strcmp(argv[1], DEBUG_KEY))
            KEY_Debug = 1;
        else if(!strcmp(argv[1],DEBUG_VFD))
            VFD_Debug = 1;
        else if(!strcmp(argv[1], DEBUG_ALL))
        {
            VFD_Debug = 1;
            KEY_Debug = 1;
        }
        else 
        {
           print_usage_panel();
           return -1;
        }
    }
    else
    {
        print_usage_panel();
        return -1;
    }
#ifdef REAL_HANGUP_PANEL
    YWPANEL_LBDStatus_T lbd = 0;
#endif
    for(;;)
    {
        if(VFD_Debug)
        {
            printf("write=========\n");
			write(ts_vfd_fd,"write", 8);
		   	write(ts_vfd_fd, "~T 11:22", WRITE_MAX_NUM);
			write(ts_vfd_fd, "~B 7", WRITE_MAX_NUM);
			write(ts_vfd_fd, "~D 1", WRITE_MAX_NUM);
			sleep(2);
			
			for(log_num=177; log_num<224; ++log_num)
			{
				sprintf(log_cmd, "~L %d 1", log_num);
				write(ts_vfd_fd, log_cmd, WRITE_MAX_NUM);
			}
			write(ts_vfd_fd,"log on", 8);
			sleep(2);
			
			for(log_num=177; log_num<224; ++log_num)
			{
				sprintf(log_cmd, "~L %d 0", log_num);
				write(ts_vfd_fd, log_cmd, WRITE_MAX_NUM);
			}
			write(ts_vfd_fd,"log off", 8);
			sleep(2);
			write(ts_vfd_fd, "~D 0", WRITE_MAX_NUM);
			write(ts_vfd_fd, "~T 00:00", WRITE_MAX_NUM);
			write(ts_vfd_fd, "", 8);
			write(ts_vfd_fd, "~B 0", WRITE_MAX_NUM);
			write(ts_vfd_fd, "ioctl",8);
			sleep(2);
			
            for(show_log.LogNum = CAB; show_log.LogNum >=PLAY_FASTBACKWARD; --show_log.LogNum)
            {
                show_log.LogSta++;
                show_log.LogSta = 1; 
				ioctl(ts_vfd_fd ,VFD_ShowLog,&show_log);
            }
			for(i=0; i<9; ++i)
			{
				ioctl(ts_vfd_fd,VFD_SetBright,&i);          
	            ioctl(ts_vfd_fd ,VFD_ShowStr,(char *)(show_str+i));
				sleep(1);
			}
			for(show_log.LogNum = CAB; show_log.LogNum >=PLAY_FASTBACKWARD; --show_log.LogNum)
            {
                show_log.LogSta++;
                show_log.LogSta = 0; 
				ioctl(ts_vfd_fd ,VFD_ShowLog,&show_log);
            }
			//ioctl(ts_vfd_fd ,VFD_ShowTime,&show_time); //1234   
#ifdef REAL_HANGUP_PANEL 
//---------------------------------------------------------------------------------------
        //YWPANEL_CPUSTATE_t cpu_state;        
        //ioctl(ts_vfd_fd,VFD_GetCPUState,&cpu_state);
        //printf("CPU_STATE is [%d]\n",cpu_state);
        u32 time;
        
        ioctl(ts_vfd_fd,VFD_GetTime,&time);
        printf("time is [0x%x]\n",time);

      
        time = 3000;
        ioctl(ts_vfd_fd,VFD_SetTime,&time);        
        printf("time is [0x%x]\n",time);
        delay(3000);
        ioctl(ts_vfd_fd,VFD_GetTime,&time);
        printf("time is [0x%x]\n",time);
        delay(3000);
        ioctl(ts_vfd_fd,VFD_GetTime,&time);
        printf("time is [0x%x]\n",time);
        time +=300;
        
        ioctl(ts_vfd_fd,VFD_SetTime,&time);
        int control = 1;//off time show
        ioctl(ts_vfd_fd,VFD_ControlTime,&control);
        delay(3000);
        
        delay(3000);
        
        delay(3000);
        //control = 1;
        //ioctl(ts_vfd_fd,VFD_ControlTime,&control);
        //YWVFD_FuncKey_T standby;
        //standby.key_index=0;
        //ioctl(ts_vfd_fd,VFD_GetBlueKey,&standby);
        //printf("bluekey[%d]=[0x%x]\n",standby.key_index,standby.key_value);
        
/*  2009-05-21@16:37:53 D02SH Remark
        ioctl(ts_vfd_fd,VFD_GetStandByKey,&standby);
        printf("standykey[%d]=[0x%x]\n",standby.key_index,standby.key_value);        
        ioctl(ts_vfd_fd,VFD_SetBlueKey,&standby);
        
        ioctl(ts_vfd_fd,VFD_GetBlueKey,&standby);
        printf("bluekey[%d]=[0x%x]\n",standby.key_index,standby.key_value);
        standby.key_value = 0x12345678;        
        ioctl(ts_vfd_fd,VFD_SetStandByKey,&standby);
*/
        lbd =(lbd++)%2;         
        ioctl(ts_vfd_fd,VFD_ControlLBD,&lbd);
        //u32 irkey;
        //ioctl(ts_vfd_fd,VFD_GetIRKey,&irkey);
        //printf("New rec a irkey =[0x%x]\n",irkey);
        
        delay(3000);
        delay(3000);
       ///cpu_state = YWPANEL_CPUSTATE_STANDBY;
        
        //ioctl(ts_vfd_fd,VFD_SetCPUState,&cpu_state);
        
#endif
		sleep(3);
        }       
        if(KEY_Debug)
        {
            read(ts_fd,&data,sizeof(data));     
            printf("Lirce Remote event type %d code= %d, value = %d \n",data.type, data.code,data.value);
            if(data.value == 1)
            {

                if(!show_env())
                   continue;

/*  2009-01-09@14:23:48 D02SH Remark
                switch(data.code) 
                {
                    case POWER_KEY:
                        system("reboot");
                        system("reboot");
                    break;
                    case FIND_KEY:
                        system("ls -slh");
                    break;
                    case RECALL_KEY:
                        system("cd /root/bin/");
                        system("ls");
                    break;
                    case RECORD_KEY:
                        system("cd /dev/;ls -sl");
                        system("echo Welcom to My world!");
                    break;
                    case STOP_KEY:
                        system("echo I will halt the TVB");
                        system("halt");
                        system("halt");
                    break;
                    default :
                        break;            
                }
*/
            }
        }
    }
    return 0;
}

/* EOF------------------------------------------------------------------------*/

/* BOL-------------------------------------------------------------------*/
//$Log: test_panel.c,v $
//Revision 1.1.1.1  2011/07/07 10:47:58  d27lwj
//no message
//
//Revision 1.1  2010/07/14 04:56:35  d27lwj
//升级NAND FLASH时面板驱动程序
//
//Revision 1.2  2009/11/26 10:25:16  d48zm
//完善vfd_write功能，修改设置(待机/唤醒)键函数接口
//
//Revision 1.1.1.1  2009/11/04 12:59:27  d22cj
//基于stsdk-a20的STI7111平台导入
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

