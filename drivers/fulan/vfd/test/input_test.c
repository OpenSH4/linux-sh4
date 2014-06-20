/*$Source: D:/STB_BASE_HAL/vfd.fulan.7167/test/input_test.c,v $*/
/*****************************�ļ�ͷ��ע��*************************************/
//
//			Copyright (C), 2008-2013, AV Frontier Tech. Co., Ltd.
//
//
// �� �� ����	$RCSfile: input_test.c,v $
//
// �� �� �ߣ�	D02SH
//
// ����ʱ�䣺	2008.11.19
//
// �����£�	$Date: 2011/07/07 10:47:58 $
//
//				$Author: d27lwj $
//
//				$Revision: 1.1.1.1 $
//
//				$State: Exp $
//
// �ļ�������	����ң��INPUT���ò���
//
/******************************************************************************/

/********************************  �ļ�����************************************/
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

/********************************  ��������************************************/
#define TS_DEV "/dev/input/event0"  //��Ҫ��������ı�
#define TS_VFD "/dev/vfd"
static int ts_fd = -1;
static int ts_vfd_fd = -1;

/********************************  ���ݽṹ************************************/

/********************************  �� �� ��************************************/

/********************************  ��������************************************/

/********************************  ��������************************************/

/********************************  ��������************************************/

/********************************  ��������************************************/
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
//����NAND FLASHʱ�����������
//
//Revision 1.1.1.1  2009/05/31 08:01:47  d02sh
//no message
//
//Revision 1.1.1.1  2009/04/03 07:19:14  d22cj
//sparkdrv_stsdk���״ε��룬��sparkdrv_0_4_00��Ӧ
//
//Revision 1.2  2008/12/02 05:47:01  d02sh
//�������֣ƣ���Ӧ�Ĳ���
//
//Revision 1.4  2008/12/01 11:34:31  d02sh
//�޸�ң�ذ����ģɣΣУգ���Ӧ��ʽ
//�֣ƣ������ʾ���ֵģ£գ�
//
//Revision 1.3  2008/11/21 12:46:36  d02sh
//����IOCTL[����������]
//
//Revision 1.2  2008/11/21 12:10:11  d02sh
//������[���ܴ�����]
//
//Revision 1.1  2008/11/19 08:42:41  d02sh
//����ң��[����]ͬʱ�������[���ڵ���]
//
//Revision 1.2  2008/11/19 08:25:49  d02sh
//ң�������Դ���
//
/* EOL-------------------------------------------------------------------*/

