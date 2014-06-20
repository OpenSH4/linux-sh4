/*$Source: D:/STB_BASE_HAL/vfd.fulan.7167/test/panel.h,v $*/
/*****************************�ļ�ͷ��ע��*************************************/
//
//			Copyright (C), 2008-2013, AV Frontier Tech. Co., Ltd.
//
//
// �� �� ����	$RCSfile: panel.h,v $
//
// �� �� �ߣ�	D02SH
//
// ����ʱ�䣺	2008.12.03
//
// �����£�	$Date: 2011/07/07 10:47:58 $
//
//				$Author: d27lwj $
//
//				$Revision: 1.1.1.1 $
//
//				$State: Exp $
//
// �ļ�������	���ⲿʹ�õĹ���ң�غ�VFD��ͷ�ļ�
//
/******************************************************************************/

#ifndef __PANEL_H
#define __PANEL_H

/********************************  �ļ�����************************************/


#ifdef __cplusplus
extern "C" {
#endif

//#define COMBO
/********************************  ��������************************************/
#define LOG_OFF     0
#define LOG_ON      1

/********************************  ���ݽṹ************************************/
typedef enum LogNum_e
{
/*----------------------------------11G-------------------------------------*/
	PLAY_FASTBACKWARD = 11*16+1,
	PLAY_HEAD,
	PLAY_LOG,
	PLAY_TAIL,
	PLAY_FASTFORWARD,
	PLAY_PAUSE,
    REC1,
    MUTE,
    CYCLE,
    DUBI,
    CA,
    CI,
    USB,
    DOUBLESCREEN,
    REC2,
/*----------------------------------12G-------------------------------------*/
    HDD_A8 = 12*16+1,
    HDD_A7,
    HDD_A6,
    HDD_A5,
    HDD_A4,
    HDD_A3,
    HDD_FULL,
    HDD_A2,
    HDD_A1,
    MP3,
    AC3,
    TVMODE_LOG,
    AUDIO,
    ALERT,
    HDD_A9,
/*----------------------------------13G-------------------------------------*/
    CLOCK_PM = 13*16+1,
    CLOCK_AM,
    CLOCK,
    TIME_SECOND, /*ʱ���֮��ĵ�*/
    DOT2,
    STANDBY,
    TER,
    DISK_S3,
    DISK_S2,
    DISK_S1,
    DISK_S0,
    SAT,
    TIMESHIFT,
    DOT1,
    CAB,	
  /*----------------------------------end-------------------------------------*/  
    LogNum_Max
}LogNum_T;

typedef struct YWVFD_Format_s
{
  unsigned char LogNum; 
  unsigned char LogSta;      
} YWVFD_Format_T; 

typedef struct YWVFD_Time_s
{
  unsigned char hour; 
  unsigned char mint;      
} YWVFD_Time_T; 

/********************************  �� �� ��************************************/
enum
{
	POWER_KEY = 88,

    TIME_SET_KEY = 87,
	UHF_KEY = 68,
	VFormat_KEY = 67,
    MUTE_KEY = 66, 
    
    TVSAT_KEY = 65,
    MUSIC_KEY = 64,
    FIND_KEY = 63,
    FAV_KEY = 62,

    MENU_KEY = 102,//HOME
    i_KEY = 61,
    EPG_KEY = 18,
    EXIT_KEY = 48,//B
    RECALL_KEY = 30,
    RECORD_KEY = 19,	
	
	UP_KEY = 103,//UP
	DOWN_KEY= 108,//DOWN
	LEFT_KEY = 105,//LEFT
	RIGHT_KEY= 106,//RIGTHT
	SELECT_KEY = 28,//ENTER	

    PLAY_KEY = 25,
    PAGE_UP_KEY = 104,	//P_UP
    PAUSE_KEY = 22,
    PAGE_DOWN_KEY = 109,//P_DOWN

    STOP_KEY = 20,
	SLOW_MOTION_KEY = 50,
	FAST_REWIND_KEY = 33,
	FAST_FORWARD_KEY = 49,

    DOCMENT_KEY = 32,
    SWITCH_PIC_KEY = 17,
    PALY_MODE_KEY = 24,
    USB_KEY = 111,

    RADIO_KEY = 110,
    SAT_KEY = 15,
    F1_KEY = 59,
	F2_KEY = 60,	
	
	RED_KEY = 44,//Z
	GREEN_KEY = 45,//X
	YELLOW_KEY = 46,//C
	BLUE_KEY = 47,//V
};
enum
{
	KEY_DIGIT0 = 11,//����input�޸�
	KEY_DIGIT1 = 2,
	KEY_DIGIT2 = 3,
	KEY_DIGIT3 = 4,
	KEY_DIGIT4 = 5,
	KEY_DIGIT5 = 6,
	KEY_DIGIT6 = 7,
	KEY_DIGIT7 = 8,
	KEY_DIGIT8 = 9,
	KEY_DIGIT9 = 10
}; 
typedef enum YWPANEL_CPUSTATE_s
{
    YWPANEL_CPUSTATE_UNKNOW,
    YWPANEL_CPUSTATE_RUNNING = 0x01,
    YWPANEL_CPUSTATE_STANDBY,    
}YWPANEL_CPUSTATE_t; 

typedef enum YWPANEL_VFDSTATE_e
{
    YWPANEL_VFDSTATE_UNKNOW,
    YWPANEL_VFDSTATE_STANDBYOFF =0x01,
    YWPANEL_VFDSTATE_STANDBYON
}YWPANEL_VFDSTATE_t;
typedef enum YWPANEL_POWERONSTATE_e
{
    YWPANEL_POWERONSTATE_UNKNOW,
    YWPANEL_POWERONSTATE_RUNNING =0x01,     /*�ϵ�����runningģʽ*/
    YWPANEL_POWERONSTATE_CHECKPOWERBIT       /*�ϵ����״̬λ*/
}YWPANEL_POWERONSTATE_t;
typedef enum YWPANEL_LBDStatus_e
{        
    YWPANEL_LBD_STATUS_OFF,           
    YWPANEL_LBD_STATUS_ON,           
    YWPANEL_LBD_STATUS_FL                
}YWPANEL_LBDStatus_T;
typedef enum YWPANEL_STARTUPSTATE_e
{
    YWPANEL_STARTUPSTATE_UNKNOW,
    YWPANEL_STARTUPSTATE_ELECTRIFY =0x01,   /*�ϵ�����*/  
    YWPANEL_STARTUPSTATE_STANDBY,            /*��������*/
    YWPANEL_STARTUPSTATE_TIMER            /*��������*/
}YWPANEL_STARTUPSTATE_t;

typedef enum YWPANEL_LOOPSTATE_e
{
    YWPANEL_LOOPSTATE_UNKNOW,
    YWPANEL_LOOPSTATE_LOOPOFF =0x01,     
    YWPANEL_LOOPSTATE_LOOPON       
}YWPANEL_LOOPSTATE_t;



typedef struct YWPANEL_CpuState_s
{
    YWPANEL_CPUSTATE_t state; 
    
}YWPANEL_CpuState_t;

typedef struct YWVFD_FuncKey_s
{
  unsigned char key_index; 
  unsigned int  key_value;      
} YWVFD_FuncKey_T; 
/********************************  ************************************/
typedef enum YWVFD_TYPE_s
{    
  YWVFD_UNKNOW,
  YWVFD_COMMON, 
  YWVFD_STAND_BY      
} YWVFD_TYPE_t; 

typedef struct YWVFD_INFO_s
{
  YWVFD_TYPE_t vfd_type;   
} YWVFD_INFO_t; 

/********************************  ��������************************************/

/********************************  ��������************************************/

/********************************  ��������************************************/

#define BASE_VFD_PRIVATE 0x00

#define VFD_GetRevision         _IOWR('s',(BASE_VFD_PRIVATE+0),char*)
#define VFD_ShowLog             _IOWR('s',(BASE_VFD_PRIVATE+1),YWVFD_Format_T)
#define VFD_ShowTime            _IOWR('s',(BASE_VFD_PRIVATE+2),YWVFD_Time_T)
#define VFD_ShowStr             _IOWR('s',(BASE_VFD_PRIVATE+3),char*)
#define VFD_ClearTime           _IOWR('s',(BASE_VFD_PRIVATE+4),int)
#define VFD_SetBright           _IOWR('s',(BASE_VFD_PRIVATE+5),int)
#define VFD_GetCPUState         _IOWR('s',(BASE_VFD_PRIVATE+6),YWPANEL_CPUSTATE_t)
#define VFD_SetCPUState         _IOWR('s',(BASE_VFD_PRIVATE+7),YWPANEL_CPUSTATE_t)
#define VFD_GetStartUpState     _IOWR('s',(BASE_VFD_PRIVATE+8),YWPANEL_STARTUPSTATE_t)
#define VFD_GetVFDState         _IOWR('s',(BASE_VFD_PRIVATE+9),YWPANEL_VFDSTATE_t)
#define VFD_SetVFDState         _IOWR('s',(BASE_VFD_PRIVATE+10),YWPANEL_VFDSTATE_t)
#define VFD_GetPOWERONState     _IOWR('s',(BASE_VFD_PRIVATE+11),YWPANEL_POWERONSTATE_t)
#define VFD_SetPOWERONState     _IOWR('s',(BASE_VFD_PRIVATE+12),YWPANEL_POWERONSTATE_t)
#define VFD_GetTime             _IOWR('s',(BASE_VFD_PRIVATE+13),u32)
#define VFD_SetTime             _IOWR('s',(BASE_VFD_PRIVATE+14),u32)
#define VFD_ControlTime         _IOWR('s',(BASE_VFD_PRIVATE+15),int)
#define VFD_GetStandByKey       _IOWR('s',(BASE_VFD_PRIVATE+16),YWVFD_FuncKey_T)
#define VFD_SetStandByKey       _IOWR('s',(BASE_VFD_PRIVATE+17),YWVFD_FuncKey_T)
#define VFD_GetBlueKey          _IOWR('s',(BASE_VFD_PRIVATE+18),YWVFD_FuncKey_T)
#define VFD_SetBlueKey          _IOWR('s',(BASE_VFD_PRIVATE+19),YWVFD_FuncKey_T)
#define VFD_GetPowerOnTime      _IOWR('s',(BASE_VFD_PRIVATE+20),u32)
#define VFD_SetPowerOnTime      _IOWR('s',(BASE_VFD_PRIVATE+21),u32)
#define VFD_ControlLBD          _IOWR('s',(BASE_VFD_PRIVATE+22),YWPANEL_LBDStatus_T)

#ifdef __cplusplus
}
#endif


#endif  /* __PANEL_H */
/* EOF------------------------------------------------------------------------*/

/* BOL-------------------------------------------------------------------*/
//$Log: panel.h,v $
//Revision 1.1.1.1  2011/07/07 10:47:58  d27lwj
//no message
//
//Revision 1.1  2010/07/14 04:56:35  d27lwj
//����NAND FLASHʱ�����������
//
//Revision 1.3  2009/11/27 01:15:29  d22cj
//�޸Ĳ���VFD���ܣ�ʹ֧����ΪLEDʱ����壬������ɨ�����������
//
//Revision 1.2  2009/11/11 03:04:18  d48zm
//vfd�Զ���⵼��
//
//Revision 1.1.1.1  2009/11/04 12:59:06  d22cj
//����stsdk-a20��STI7111ƽ̨����
//
//Revision 1.3  2009/06/10 03:51:06  d02sh
//����COMBO֧��
//
//Revision 1.2  2009/05/08 01:54:47  d22cj
//1���޸��ĵ�sparkdrv_framebuffer.doc����������FrameBufferͼ������͸���ȵ��÷���ֵ��ע�����1080Iʱ͸���ȵ����÷�Χ�������ֱ��ʲ�ͬ���ο�testapps/framebuffer_test����Ҫ��������stgfb_core.ko
//2������automount.sh��mdev.conf�ű�����dependence
//3���޸Ĵ��ڣ�ʹ��֧��SPCS��������ں�
//
//Revision 1.1.1.1  2009/04/03 07:18:48  d22cj
//sparkdrv_stsdk���״ε��룬��sparkdrv_0_4_00��Ӧ
//
/* EOL-------------------------------------------------------------------*/

