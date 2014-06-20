/*$Source: D:/STB_BASE_HAL/vfd.fulan.7167/panel.c,v $*/
/*****************************文件头部注释*************************************/
//
//			Copyright (C), 2008-2013, AV Frontier Tech. Co., Ltd.
//
//
// 文 件 名：	$RCSfile: panel.c,v $
//
// 创 建 者：	D02SH
//
// 创建时间：	2008.11.18
//
// 最后更新：	$Date: 2011/08/03 02:51:06 $
//
//				$Author: d27lwj $
//
//				$Revision: 1.3 $
//
//				$State: Exp $
//
// 文件描述：	红外遥控和面板[需要修改相应的内核文件]stblast.c[位于src\stblast\]LINE
//				1821的情况选用下者Op->UserCount_p = SymbolsRead;
//				和1842的情况选用下者Blaster_p->SymbolBuf_p = Buffer;注释可由YWDRIVER_MODI
//				追踪
/******************************************************************************/

/********************************  文件包含************************************/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/sysrq.h>
#include <linux/serial.h>
#include <linux/init.h>
#include <linux/console.h>
#include <linux/generic_serial.h>

#include <asm/system.h>
#include <asm/io.h>
#include <asm/cacheflush.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/bitops.h>
#include <asm/clock.h>

#include <linux/input.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30)
#include <asm/semaphore.h>
#else
#include <linux/semaphore.h>
#endif

#include <linux/cdev.h>
#include <linux/kthread.h>	//内核线程等相关函数
#include <linux/delay.h>	//sleep等相关函数
#include <linux/ctype.h>
#include <linux/string.h>

#include "trace.h"

#include "panel.h"
#include "panel_iner.h"

#include <linux/time.h>
/********************************  常量定义************************************/
#define DEV_MAJOR_VFD  0//246								//主设备号
static int vfd_major = DEV_MAJOR_VFD;
#define vfd_devs_nm  	0									//vfd设备数量
//增加遥控与面板的按键互斥
#define KEY_MUTEX_NEED
#ifdef KEY_MUTEX_NEED
struct semaphore key_mutex ;
#endif
//----定义相关按键状态
#define NO_KEY_PRESS	-1
#define KEY_PRESS_DOWN 1
#define KEY_PRESS_UP   0
//-----红外接收相关情况
#define REC_NEW_KEY 34
#define REC_NO_KEY	0
#define REC_REPEAT_KEY	2
//#ifdef COMBO
#define STORN_KEY_NUM 5
static YWVFD_FuncKey_T YWVFD_StandByKey[STORN_KEY_NUM];
static YWVFD_FuncKey_T YWVFD_BlueKey[STORN_KEY_NUM];
//#endif

#define WRITE_MIN_NUM   3
#define WRITE_MAX_NUM	256

//---------------------------创建相应的线程--------------------------------------------
struct task_struct *KeyboardTASK;
struct task_struct *RemoteTASK;


#define SYMBOL_BUFFER_SIZE				200

#define	REMOTE_KEY_ADDR_MASK			0xFFFF0000	/* based on he 11 bit value */
#define	REMOTE_KEY_DATA_MASK			0xFFFF	/* based on the 11 bit value */
#define	REMOTE_KEY_TOGGLE_BIT_MASK		0x200	/* based on the 11 bit value */

#define	REMOTE_KEY_BUFFER_SIZE			20
#define	REMOTE_KEY_LOGIC_0				0xFE
#define	REMOTE_KEY_LOGIC_1				0x01
#define	DATA_LOGIC1_DOWN_LIMIT			1500
#define	DATA_LOGIC1_UP_LIMIT			2500


#define VFD_SPIN 1	//自旋锁开关
int VFD_count = 0 ;
spinlock_t VFD_lock ;
struct semaphore big_vfd_sem;

/********************************  数据结构************************************/
struct panel_lirc
{
	struct input_dev	*panel_input;
	struct cdev 		vfd_cdev ;/*cdev结构体*/
	//char				phys[32];
};

/********************************  宏 定 义************************************/
//利用cvs获取可以更新的版本号
#define PANEL_MODULE_VERSION	"$Revision: 1.3 $#$State: Exp $"

/********************************  变量定义************************************/
/********************************  变量引用************************************/
struct panel_lirc *panel_hp ;

/********************************  函数定义************************************/

/*****************************************************************************/
//函数名称:	DelCharFromeStr
//函数说明:	从字符串中删除相应的字符
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	无
//输出参数:	无
//返 回 值:	无
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
/*****************************************************************************/
static void DelCharFromStr(char s[],char ch)
{
	char* q = s;
	for(;*q;q++)
		if(*q != ch)
			*(s++) = *q;
	*s = 0;
}

#ifdef YWPANEL_KEYBOARD  //lwj add
/*****************************************************************************/
//函数名称:	YWPANEL_Keyboard_Process
//函数说明:	面板按键扫描和遥控监控进程
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	无
//输出参数:	无
//返 回 值:	无
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
//			 2009.11.10        曾鸣           修 改
/*****************************************************************************/
int YWPANEL_Keyboard_Process ( void *pvParam )
{
	int iKeyboardKeyRead = INVALID_KEY ,prekey = INVALID_KEY;
	int keyup_flag = KEY_PRESS_UP;
	struct input_dev *panel_dev = panel_hp->panel_input;
	while (!kthread_should_stop())
	{
		iKeyboardKeyRead = YWPANEL_VFD_GetKeyValue();

		if(iKeyboardKeyRead == INVALID_KEY)
		{
			if(keyup_flag == KEY_PRESS_DOWN)
			{
				PANEL_PRINT((TRACE_ERROR,"key %d unpress!\n",prekey));
				input_report_key(panel_dev,prekey,KEY_PRESS_UP);
#ifdef KEY_MUTEX_NEED
				up(&key_mutex);
#endif
				YWPANEL_VFD_ShowIco(DOT2,LOG_OFF);
				prekey = iKeyboardKeyRead;
				keyup_flag = KEY_PRESS_UP;
			}
			msleep(80);
			continue;
		}
		else if(iKeyboardKeyRead != prekey)
		{
			if(keyup_flag == KEY_PRESS_DOWN)
			{
				input_report_key(panel_dev,prekey,KEY_PRESS_UP);
				YWPANEL_VFD_ShowIco(DOT2,LOG_OFF);
				prekey = iKeyboardKeyRead;
				keyup_flag = KEY_PRESS_UP;
#ifdef KEY_MUTEX_NEED
				up(&key_mutex);
#endif
			}

#ifdef KEY_MUTEX_NEED
			//down(&key_mutex);
			if (down_interruptible(&key_mutex))
			{
			   continue;
			}
#endif
			input_report_key(panel_dev,iKeyboardKeyRead,KEY_PRESS_DOWN);
			YWPANEL_VFD_ShowIco(DOT2,LOG_ON);
			prekey = iKeyboardKeyRead;
			keyup_flag = KEY_PRESS_DOWN;
		}
		else
		{
			msleep(80);
			continue;
		}
	}
   PANEL_PRINT((TRACE_ERROR,"kthread YWPANEL_Keyboard_Process stop!\n"));
   return 0 ;
}
#endif

/*****************************************************************************/
//函数名称:	YWPANEL_Thread_Init
//函数说明:	开启相应的线程，主要是扫描按键情况
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	无
//输出参数:	无
//返 回 值:	无
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
/*****************************************************************************/
int	YWPANEL_Thread_Init ( void )
{
	int	   ErrorCode = 0;
#ifdef KEY_MUTEX_NEED
	init_MUTEX(&key_mutex);
#endif
#ifdef YWPANEL_KEYBOARD
	KeyboardTASK = kthread_run(YWPANEL_Keyboard_Process, NULL, "Keyboard_thread ");
#endif
	return ErrorCode;
}
/**********************************************************************************************/
/*************						   LINUX 驱动					   ************************/
/**********************************************************************************************/

static int vfd_open(struct inode *inode,struct file *filp)
{
	filp->private_data = panel_hp;
#if VFD_SPIN
	spin_trylock(&VFD_lock);
	PANEL_PRINT((TRACE_INFO, "^!^ you opened me!\n "));
	VFD_count++ ;  /*增加使用计数*/
	spin_unlock(&VFD_lock);
#endif
	return 0;
}
static int vfd_release(struct inode *inode,struct file *filp)
{
#if VFD_SPIN
	spin_trylock(&VFD_lock);
	VFD_count-- ;  /*减少使用计数*/
	spin_unlock(&VFD_lock);
#endif
	PANEL_PRINT((TRACE_ERROR, "^!^ see you later  !\n "));
	return 0;
}

/*read函数*/
static int vfd_read(struct file *filp,char *buff,size_t count,loff_t *offp)
{
	return 0;
}

/*write函数
已知支持的特殊字符为^*-_+[]\,./
*/
static int vfd_write(struct file *filp,const char *buff,size_t count,loff_t *offp)
{
	char str[WRITE_MAX_NUM+1];	//最后一位补'\0'
	u32 mul_num;
	ssize_t ret = count;	//因echo命令会等待直到vfd_write返回count，而vfd无法都显示
	int i;

	if (down_interruptible(&big_vfd_sem))
    {
    	ret = -EBUSY;
		goto out;
    }

	if(get_user(str[0], (char __user*)buff) !=0)
	{
		ret = -EFAULT;
		goto out;
	}

	if('~' != str[0])
	{
		if(count > 8)
		{
			count = 8;
		}

		if (copy_from_user(str, (void __user*)buff, count) != 0)
		{
		    ret = -EFAULT;
		    goto out;
		}
		str[count] = '\0';    //防止后面strlen(str)时出错
		YWPANEL_VFD_ShowString(str);
	}
	else
	{
		if(count > WRITE_MAX_NUM)
		{
			count = WRITE_MAX_NUM;
		}
		if (copy_from_user(str, (void __user*)buff, count) !=0)
		{
		    ret = -EFAULT;
		    goto out;
		}
		if(count < WRITE_MIN_NUM)
		{
			ret = -EINVAL;
			goto out;
		}
		str[count] = '\0';
		mul_num = 0;
		i = 2;

		switch(str[1])
		{
			case 'l':	//与'L'相同处理
			case 'L':	//show log   //~L [177-224] [0-1]
			{
				YWVFD_Format_T	show_log;
				show_log.LogNum = 0;

				while(' '==str[i])
				{
					++i;
				}
				while(isdigit(str[i]))
				{
					show_log.LogNum *= mul_num;
					show_log.LogNum += str[i]-'0';	//LogNum为u8类型未做溢出判断
					mul_num = 10;
					++i;
				}

				if(str[i++] != ' ')
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				while(' '==str[i])
				{
					++i;
				}

				if(!isdigit(str[i]))
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				else
				{
					switch(str[i])
					{
						case '1':
						{
							show_log.LogSta = 1;
							YWPANEL_VFD_ShowIco(show_log.LogNum,show_log.LogSta);
							break;
						}
						case '0':
						{
							show_log.LogSta = 0;
							YWPANEL_VFD_ShowIco(show_log.LogNum,show_log.LogSta);
							break;
						}
						default:
						{
							PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
							break;
						}
					}
				}
				break;
			}
			case 't':	//大小写作相同处理
			case 'T':	//show time
			{
				u32 time;
				//~T [00-24] [00-59]
				while(' '==str[i])
				{
					++i;
				}

				if(!(str[i]>='0'&&str[i]<='2') || !(str[i+1]>='0'&&str[i+1]<='3'))
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid\n"));
					goto out;
				}
				time = (str[i]-'0')*10;	//hour
				time += str[++i]-'0';
				time *= 60;	//hour to min
				i += 2;	//ignore ':'

				if(!(str[i]>='0'&&str[i]<='5') || !(str[i+1]>='0'&&str[i+1]<='9'))
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid\n"));
					goto out;
				}
				time += (str[i]-'0')*10;//min
				time += str[++i]-'0';
				time *= 60;	//min to sec

				++i;
				while(' '==str[i])
				{
					++i;
				}

				if(strncasecmp("~P", str+i, 2)==0)
				{
					YWPANEL_FP_SetPowerOnTime(time);
				}
				else
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid\n"));
					YWPANEL_FP_SetTime(time);
				}
				break;
			}
			case 'b':	//与'B'相同处理
			case 'B':  //~B [0-7] SetBright
			{
				int level;
				while(' '==str[i])
				{
					++i;
				}
				if(!isdigit(str[i]))
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				level = str[i]-'0';	//只取一位 因level范围为0-7
				YWPANEL_VFD_SetBrightness(level);
				break;
			}
			case 'c':
			case 'C':	//~C [0-2]为SetCPUState
			{
				YWPANEL_CPUSTATE_t cpu_state;
				while(' '==str[i])
				{
					++i;
				}
				cpu_state = str[i]-'0';
				if(cpu_state<YWPANEL_CPUSTATE_UNKNOW || cpu_state>YWPANEL_CPUSTATE_STANDBY)
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				YWPANEL_FP_SetCpuStatus(cpu_state);
				break;
			}
			case 'v':
			case 'V':	//~V [0-2]为SetVFDState
			{
				YWPANEL_VFDSTATE_t vfd_state;
				while(' '==str[i])
				{
					++i;
				}
				vfd_state = str[i]-'0';
				if(vfd_state<YWPANEL_VFDSTATE_UNKNOW || vfd_state>YWPANEL_VFDSTATE_STANDBYON)
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				YWPANEL_FP_SetVFDStatus(vfd_state);
				break;
			}
			case 'p':
			case 'P':	//~P [0-2]为SetPOWERONState
			{
				YWPANEL_POWERONSTATE_t poweron_state;
				while(' '==str[i])
				{
					++i;
				}
				poweron_state = str[i]-'0';
				if(poweron_state<YWPANEL_POWERONSTATE_UNKNOW || poweron_state>YWPANEL_POWERONSTATE_CHECKPOWERBIT)
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				YWPANEL_FP_SetPowerOnStatus(poweron_state);
				break;
			}
			case 'd':
			case 'D':	//~D [0-2]为ControlLBD
			{
				YWPANEL_LBDStatus_T state;
				while(' '==str[i])
				{
					++i;
				}
				state = str[i]-'0';
				if(state<YWPANEL_LBD_STATUS_OFF || state>YWPANEL_LBD_STATUS_FL)
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				YWPANEL_LBD_SetStatus(state);
				break;
			}
			case 'i':
			case 'I':	//~I [0-5] ~V [0-255] ~S为SetStandByKey ~I [0-5] ~V [0-255] ~B为SetBlueKey
			{
				YWVFD_FuncKey_T key;

				memset(&key, 0, sizeof(key));
				while(' '==str[i])
				{
					++i;
				}
				if(!isdigit(str[i]))
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}
				key.key_index = str[i]-'0';
				if(key.key_index > STORN_KEY_NUM)
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}

				++i;
				while(' '==str[i])
				{
					++i;
				}
				if(strncasecmp("~V", str+i, 2)!=0)
				{
					goto out;
				}

				i+=2;
				while(' '==str[i])
				{
					++i;
				}
				mul_num = 0;
				while(isdigit(str[i]))
				{
					key.key_value *= mul_num;
					key.key_value += str[i]-'0';	//LogNum为u8类型未做溢出判断
					mul_num = 10;
					++i;
				}
				if(0 == mul_num)
				{
					PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
					goto out;
				}

				while(' '==str[i])
				{
					++i;
				}
				if(strncasecmp("~S", str+i, 2)==0)
				{
					if(!YWPANEL_FP_SetStandByKey(key.key_index,key.key_value))
					{
						goto out;
					}
					YWVFD_StandByKey[key.key_index].key_value = key.key_value;
				}
				else if(strncasecmp("~B", str+i, 2)==0)
				{
					if(!YWPANEL_FP_SetBlueKey(key.key_index,key.key_value))
					{
						goto out;
					}
					YWVFD_BlueKey[key.key_index].key_value = key.key_value;
				}
				else
				{
					goto out;
				}
				break;
			}
			default:
			{
				PANEL_PRINT((TRACE_ERROR, "Command Invalid!\n"));
				break;
			}
		}
	}

out:
	up(&big_vfd_sem);
	return ret;
}

static int vfd_ioctl(struct inode *inode,struct file *file,unsigned int
					cmd,unsigned long arg)
{
	if (down_interruptible(&big_vfd_sem))
    {
       return -EBUSY;
    }
	switch(cmd)
	{
		case VFD_GetRevision:
		{
			char Revision[32];
            YWPANEL_VFD_GetRevision(Revision);
			DelCharFromStr(Revision,'$');
			if(copy_to_user((char *)arg,  Revision, sizeof(Revision)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_ShowLog:
		{
			YWVFD_Format_T	show_log;
			if(copy_from_user(&show_log, (YWVFD_Format_T *)arg, sizeof(YWVFD_Format_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_ShowLog error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_VFD_ShowIco(show_log.LogNum,show_log.LogSta);
			}

			break;
		}
		case VFD_ShowTime:
		{
			YWVFD_Time_T  show_Time;
			if(copy_from_user(&show_Time,(YWVFD_Time_T *)arg, sizeof(YWVFD_Time_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_ShowTime error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_VFD_ShowTime(show_Time.hour,show_Time.mint);
			}

			break;
		}
		case VFD_ClearTime:
		{
			YWPANEL_VFD_ShowTimeOff();
			break;
		}
		case VFD_ShowStr:
		{
			char* show_str = (char *)arg;
			YWPANEL_VFD_ShowString(show_str);

			break;
		}
//add for standby d02sh 090520
		case VFD_SetBright:
		{
			int level;
			if(copy_from_user(&level, (int *)arg, sizeof(int)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_SetBright error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_VFD_SetBrightness(level);
			}
			break;
		}
		case VFD_GetCPUState:
		{
			YWPANEL_CPUSTATE_t cpu_state;
			cpu_state = YWPANEL_FP_GetCpuStatus();
			if(copy_to_user((YWPANEL_CPUSTATE_t *)arg,  &cpu_state, sizeof(YWPANEL_CPUSTATE_t)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_SetCPUState:
		{
			YWPANEL_CPUSTATE_t cpu_state;
			if(copy_from_user(&cpu_state, (YWPANEL_CPUSTATE_t *)arg, sizeof(YWPANEL_CPUSTATE_t)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_SetCPUState error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_FP_SetCpuStatus(cpu_state);
			}
			break;
		}
		case VFD_GetStartUpState:
		{
			YWPANEL_STARTUPSTATE_t startup_state;

            YWPANEL_FP_GetStartUpState(&startup_state);
			if(copy_to_user((YWPANEL_STARTUPSTATE_t *)arg,  &startup_state, sizeof(YWPANEL_STARTUPSTATE_t)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_GetVFDState:
		{
			YWPANEL_VFDSTATE_t vfd_state;
			vfd_state = YWPANEL_FP_GetVFDStatus();
			if(copy_to_user((YWPANEL_VFDSTATE_t *)arg,  &vfd_state, sizeof(YWPANEL_VFDSTATE_t)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_SetVFDState:
		{
			YWPANEL_VFDSTATE_t vfd_state;
			if(copy_from_user(&vfd_state, (YWPANEL_VFDSTATE_t *)arg, sizeof(YWPANEL_VFDSTATE_t)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_SetVFDState error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_FP_SetVFDStatus(vfd_state);
			}
			break;
		}
		case VFD_GetPOWERONState:
		{
			YWPANEL_POWERONSTATE_t poweron_state;
			poweron_state = YWPANEL_FP_GetPowerOnStatus();
			if(copy_to_user((YWPANEL_POWERONSTATE_t *)arg,  &poweron_state, sizeof(YWPANEL_POWERONSTATE_t)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_SetPOWERONState:
		{
			YWPANEL_POWERONSTATE_t poweron_state;
			if(copy_from_user(&poweron_state, (YWPANEL_POWERONSTATE_t *)arg, sizeof(YWPANEL_POWERONSTATE_t)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_SetVFDState error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_FP_SetPowerOnStatus(poweron_state);
			}
			break;
		}
		case VFD_GetTime:
		{
			u32 time;
			time = YWPANEL_FP_GetTime();
			if(copy_to_user((u32 *)arg,  &time, sizeof(u32)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_SetTime:
		{
			u32 time;
			if(copy_from_user(&time, (u32 *)arg, sizeof(u32)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_SetTime error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_FP_SetTime(time);
			}
			break;
		}
		case VFD_ControlTime:
		{
			int state;
			if(copy_from_user(&state, (int *)arg, sizeof(int)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_ControlTime error arg !\n"));
				goto out_efault;
			}
			else
			{
				if((state < 0)||(state > 1))
					PANEL_PRINT((TRACE_ERROR,"VFD_ControlTime error arg !\n"));
				else
					YWPANEL_FP_ControlTimer(state);
			}
			break;
		}
		case VFD_GetStandByKey:
		{
			YWVFD_FuncKey_T standby_key;
			if(copy_from_user(&standby_key, (YWVFD_FuncKey_T *)arg, sizeof(YWVFD_FuncKey_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_GetStandByKey error arg !\n"));
				goto out_efault;
			}
			else
			{
				if(standby_key.key_index > STORN_KEY_NUM)
					PANEL_PRINT((TRACE_ERROR,"VFD_ControlTime error arg !\n"));
				else
					standby_key.key_value = YWPANEL_FP_GetStandByKey(standby_key.key_index);
			}
			if(copy_to_user((YWVFD_FuncKey_T *)arg,  &standby_key, sizeof(YWVFD_FuncKey_T)))
			{
				goto out_efault;
			}
			YWVFD_StandByKey[standby_key.key_index].key_value= standby_key.key_value;
			break;
		}
		case VFD_SetStandByKey:
		{
			YWVFD_FuncKey_T standby_key;
			bool ret = false;
			if(copy_from_user(&standby_key, (YWVFD_FuncKey_T *)arg, sizeof(YWVFD_FuncKey_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_GetStandByKey error arg !\n"));
				goto out_efault;
			}
			else
			{
				if(standby_key.key_index > STORN_KEY_NUM)
					PANEL_PRINT((TRACE_ERROR,"VFD_ControlTime error arg !\n"));
				else
					ret = YWPANEL_FP_SetStandByKey(standby_key.key_index,standby_key.key_value);
			}
			if(ret == true)
				YWVFD_StandByKey[standby_key.key_index].key_value = standby_key.key_value;
			break;
		}
		case VFD_GetBlueKey:
		{
			YWVFD_FuncKey_T standby_key;
			if(copy_from_user(&standby_key, (YWVFD_FuncKey_T *)arg, sizeof(YWVFD_FuncKey_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_GetStandByKey error arg !\n"));
				goto out_efault;
			}
			else
			{
				if(standby_key.key_index > STORN_KEY_NUM)
					PANEL_PRINT((TRACE_ERROR,"VFD_ControlTime error arg !\n"));
				else
					standby_key.key_value = YWPANEL_FP_GetBlueKey(standby_key.key_index);
			}
			if(copy_to_user((YWVFD_FuncKey_T *)arg,  &standby_key, sizeof(YWVFD_FuncKey_T)))
			{
				goto out_efault;
			}
			YWVFD_BlueKey[standby_key.key_index].key_value = standby_key.key_value;
			break;
		}
		case VFD_SetBlueKey:
		{
			YWVFD_FuncKey_T standby_key;
			bool  ret = false;
			if(copy_from_user(&standby_key, (YWVFD_FuncKey_T *)arg, sizeof(YWVFD_FuncKey_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_GetStandByKey error arg !\n"));
				goto out_efault;
			}
			else
			{
				if(standby_key.key_index > STORN_KEY_NUM)
					PANEL_PRINT((TRACE_ERROR,"VFD_ControlTime error arg !\n"));
				else
					ret = YWPANEL_FP_SetBlueKey(standby_key.key_index,standby_key.key_value);
			}
			if(ret == true)
				YWVFD_BlueKey[standby_key.key_index].key_value = standby_key.key_value;
			break;
		}
		case VFD_SetPowerOnTime:
		{
			u32 time;
			if(copy_from_user(&time, (u32 *)arg, sizeof(u32)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_SetTime error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_FP_SetPowerOnTime(time);
			}
			break;
		}
		case VFD_GetPowerOnTime:
		{
			u32 time;
			time = YWPANEL_FP_GetPowerOnTime();
			if(copy_to_user((u32 *)arg,  &time, sizeof(u32)))
			{
				goto out_efault;
			}
			break;
		}
		case VFD_ControlLBD:
		{
			YWPANEL_LBDStatus_T state;
			if(copy_from_user(&state, (YWPANEL_LBDStatus_T *)arg, sizeof(YWPANEL_LBDStatus_T)))
			{
				PANEL_PRINT((TRACE_ERROR,"VFD_ControlLBD error arg !\n"));
				goto out_efault;
			}
			else
			{
				YWPANEL_LBD_SetStatus(state);
			}
			break;
		}
/*	2009-06-02@10:00:19 D02SH Remark
		case VFD_GetIRKey:
		{
			u32 keyvalue;
			keyvalue = YWPANEL_FP_GetIRKey();
			if(copy_to_user(arg,  &keyvalue, sizeof(u32)))
			{
				return	-EFAULT;
			}
			break;
		}
*/
		default :
		{
			PANEL_PRINT((TRACE_ERROR,"NO Command [0x%x]!\n",cmd));
			break;
		}

out_efault:
	up(&big_vfd_sem);
	return -EFAULT;
	}

	//break to here
	up(&big_vfd_sem);
	return 0;
}
 /*相应的文件操作*/
static const struct file_operations vfd_fops=
{
   .owner	= THIS_MODULE,
   .ioctl	= vfd_ioctl,
   .open	= vfd_open,
   .release = vfd_release ,
   .read	= vfd_read ,
   .write	= vfd_write ,
};
/*****************************************************************************/
//函数名称:	vfd_setup_cdev
//函数说明:	初始化并注册cdev
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	无
//输出参数:	无
//返 回 值:	无
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
/*****************************************************************************/
static int vfd_setup_cdev(void)
{
	int result;
	dev_t dev_vfd = MKDEV(vfd_major, 0);
	/*申请设备号*/
	if (vfd_major)
	{
		result = register_chrdev_region(dev_vfd, vfd_devs_nm, "vfd");
	}
	else /*动态申请设备号*/
	{
		result = alloc_chrdev_region(&dev_vfd, 0, vfd_devs_nm, "vfd");
		vfd_major = MAJOR(dev_vfd);
	}
	if (result < 0)
	{
		PANEL_PRINT((TRACE_INFO, "VFD: can't get major %d\n", vfd_major));
	}
    else
    {
        printk("vfd_setup_cdev SUCCESSFUL ======\n");
    }
	cdev_init(&panel_hp->vfd_cdev, &vfd_fops);
	panel_hp->vfd_cdev.owner = THIS_MODULE;
	panel_hp->vfd_cdev.ops = &vfd_fops;
	result = cdev_add(&panel_hp->vfd_cdev, dev_vfd, 1);
	if (result)
		PANEL_PRINT((TRACE_ERROR,"Error %d adding VFD", result));
	return result ;
}



/*****************************************************************************/
//函数名称:	YWPANEL_Input_Init
//函数说明:	初始化Input设备
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	panel_input
//输出参数:	无
//返 回 值:	无
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
/*****************************************************************************/
void YWPANEL_Input_Init(struct input_dev *panel_input)
{
    return;
    #if 0
	panel_input->private = panel_hp;
	panel_input->name = "panel & remote use lirc ";
//	panel_input->phys = panel_hp->phys;
	panel_input->phys = "/dev/input/event0";
	panel_input->id.bustype = BUS_RS232;  //不知道有没有影响
	panel_input->id.vendor = 0x00;
	panel_input->id.product = 0x0051;
	panel_input->id.version = 0x0100;
	panel_input->evbit[0] = BIT(EV_KEY);

	panel_input->keybit[LONG(BTN_TOUCH)] = BIT(BTN_TOUCH) ;

	set_bit(POWER_KEY , 				panel_input->keybit);

	set_bit(TIME_SET_KEY ,				panel_input->keybit);
	set_bit(UHF_KEY ,					panel_input->keybit);
	set_bit(VFormat_KEY,				panel_input->keybit);
	set_bit(MUTE_KEY,					panel_input->keybit);

	set_bit(TVSAT_KEY , 				panel_input->keybit);
	set_bit(MUSIC_KEY , 				panel_input->keybit);
	set_bit(FIND_KEY ,					panel_input->keybit);
	set_bit(FAV_KEY ,					panel_input->keybit);

	set_bit(KEY_DIGIT0 ,				panel_input->keybit);
	set_bit(KEY_DIGIT1 ,				panel_input->keybit);
	set_bit(KEY_DIGIT2 ,				panel_input->keybit);
	set_bit(KEY_DIGIT3 ,				panel_input->keybit);
	set_bit(KEY_DIGIT4 ,				panel_input->keybit);
	set_bit(KEY_DIGIT5 ,				panel_input->keybit);
	set_bit(KEY_DIGIT6 ,				panel_input->keybit);
	set_bit(KEY_DIGIT7 ,				panel_input->keybit);
	set_bit(KEY_DIGIT8 ,				panel_input->keybit);
	set_bit(KEY_DIGIT9 ,				panel_input->keybit);

	set_bit(MENU_KEY ,					panel_input->keybit);
	set_bit(i_KEY , 					panel_input->keybit);
	set_bit(EPG_KEY ,					panel_input->keybit);
	set_bit(EXIT_KEY ,					panel_input->keybit);
	set_bit(RECALL_KEY ,				panel_input->keybit);
	set_bit(RECORD_KEY ,				panel_input->keybit);

	set_bit(UP_KEY ,					panel_input->keybit);
	set_bit(DOWN_KEY ,					panel_input->keybit);
	set_bit(LEFT_KEY,					panel_input->keybit);
	set_bit(RIGHT_KEY,					panel_input->keybit);
	set_bit(SELECT_KEY, 				panel_input->keybit);

	set_bit(PLAY_KEY,					panel_input->keybit);
	set_bit(PAGE_UP_KEY,				panel_input->keybit);
	set_bit(PAUSE_KEY,					panel_input->keybit);
	set_bit(PAGE_DOWN_KEY,				panel_input->keybit);

	set_bit(STOP_KEY,					panel_input->keybit);
	set_bit(SLOW_MOTION_KEY,			panel_input->keybit);
	set_bit(FAST_REWIND_KEY,			panel_input->keybit);
	set_bit(FAST_FORWARD_KEY,			panel_input->keybit);

	set_bit(DOCMENT_KEY,				panel_input->keybit);
	set_bit(SWITCH_PIC_KEY, 			panel_input->keybit);
	set_bit(PALY_MODE_KEY,				panel_input->keybit);
	set_bit(USB_KEY,					panel_input->keybit);

	set_bit(RADIO_KEY,					panel_input->keybit);
	set_bit(SAT_KEY,					panel_input->keybit);
	set_bit(F1_KEY, 					panel_input->keybit);
	set_bit(F2_KEY, 					panel_input->keybit);

	set_bit(RED_KEY,					panel_input->keybit);
	set_bit(GREEN_KEY,					panel_input->keybit);
	set_bit(YELLOW_KEY, 				panel_input->keybit);
	set_bit(BLUE_KEY,					panel_input->keybit);
    #endif
}


/*****************************************************************************/
//函数名称:	panel_init
//函数说明:	panel驱动初始化函数
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	无
//输出参数:	无
//返 回 值:	0：成功。1：失败
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
/*****************************************************************************/
static int __init panel_init(void)
{
	int	ret = 0;
	#if 0
	struct input_dev *panel_input_dev;
	#endif  /* 0 */
	//--------申请空间--------
	panel_hp = kzalloc(sizeof(struct panel_lirc), GFP_KERNEL);
	if (!panel_hp) {
		ret = -ENOMEM;
		PANEL_PRINT((TRACE_ERROR,"kzalloc panel_lirc err\n"));
	}
	//----------注册字符设备
	spin_lock_init(&VFD_lock);
	init_MUTEX(&big_vfd_sem);
	ret = vfd_setup_cdev();
	if(1 == ret)
	{
		goto fail_vfd_setup_cdev;
	};
	//----------注册INPUT设备
	#if 0
	panel_input_dev = input_allocate_device();
	if (!panel_input_dev) {
		ret = -ENOMEM;
		PANEL_PRINT((TRACE_ERROR,"input_allocate_device err\n"));
		goto fail_allocate_input;
	}
	YWPANEL_Input_Init(panel_input_dev);

	panel_hp->panel_input= panel_input_dev;

	ret = input_register_device(panel_hp->panel_input);
	if (ret)
	{
		PANEL_PRINT((TRACE_ERROR,"input_register_device error!\n"));
		goto fail_register_input;
	}
	PANEL_PRINT((TRACE_ERROR,"reg input ok!!!\n"));
	#endif
	ret = YWPANEL_VFD_Init();
	if(ret != 0)
	{
		PANEL_DEBUG(ret);
		goto fail_YWVFD_Init ;
	}

#ifdef YWPANEL_KEYBOARD  //lwj add
	ret = YWPANEL_Thread_Init();  //I don't need this
	if(ret != 0)
	{
		PANEL_DEBUG(ret);
		goto fail_YWPANEL_Thread_Init;
	}
#endif

	return ret ;

#ifdef YWPANEL_KEYBOARD  //lwj add
fail_YWPANEL_Thread_Init:
	YWPANEL_VFD_Term();
#endif

fail_YWVFD_Init:
	{
		input_unregister_device(panel_hp->panel_input);
	}
#if 0
fail_register_input :
#endif  /* 0 */
	{
		input_free_device(panel_hp->panel_input);
	}
#if 0
fail_allocate_input :
#endif  /* 0 */
	{
		cdev_del(&panel_hp->vfd_cdev);
		unregister_chrdev_region(MKDEV(vfd_major,0),1);
	}
fail_vfd_setup_cdev:
	kfree(panel_hp);
	return ret;
}
/*****************************************************************************/
//函数名称:	panel_exit
//函数说明:	panel驱动卸载函数
//算法说明:	无
//调用说明:   Calls 							Called By
//			 ------ 						  ------------
//			  AAAAA()							BBBBB()
//输入参数:	无
//输出参数:	无
//返 回 值:	无
//修改记录:	日		 期 	 作 	 者 	  修 定
//			 ---------		 ---------- 	 -------
//			 2008.12.23 	   D02SH		  新 建
/*****************************************************************************/
static void __exit panel_exit(void)
{
	//添加相应的资源释放
	int ST_ErrCode = 0;
	PANEL_PRINT((TRACE_ERROR,"module exit!\n"));

#ifdef YWPANEL_KEYBOARD  //lwj add
	kthread_stop(KeyboardTASK);  //I don't need this
#endif
	ST_ErrCode = YWPANEL_VFD_Term();
	if(ST_ErrCode != 0)
	{
		PANEL_DEBUG(ST_ErrCode);
	}
	//input设备的卸载
	//input_unregister_device(panel_hp->panel_input);
	//input_free_device(panel_hp->panel_input);
	 //字符设备的卸载
	cdev_del(&panel_hp->vfd_cdev);
	unregister_chrdev_region(MKDEV(vfd_major,0),1);
	kfree(panel_hp);
	PANEL_PRINT((TRACE_ERROR,"the module pane is moveout!\n"));
	return ;
}

module_init(panel_init);
module_exit(panel_exit);

MODULE_AUTHOR("<rd@fulan.vicp.cc>");
MODULE_DESCRIPTION("ST7101_PANEL DRV for HP");
MODULE_VERSION(PANEL_MODULE_VERSION);
MODULE_LICENSE("fulansoft");
 /* EOF------------------------------------------------------------------------*/

