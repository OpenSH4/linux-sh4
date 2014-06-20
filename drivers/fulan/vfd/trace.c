
/**********************************�ļ�ͷ��ע��************************************/
//
//
//  					Copyright (C), 2005-2010, AV Frontier Tech. Co., Ltd.
//
//
// �ļ�����		trace.c
//
// �����ߣ�		mjj
//
// �������ڣ�	2007/12/12
//
// �ļ�������	[T]ask [R]eporting [A]nd [C]urrent [E]valuation
//
// ��ǰά���ߣ�(  ��API  ��ָ����Ա����ά��)
//
//  �����£�	2007/12/12
//
//												 
/*****************************************************************************************/


/************************************�ļ�����****************************************/
#include <linux/module.h>

//#include "ywdefs.h"
#include "trace.h"

/************************************�궨��*******************************************/


/************************************���ݽṹ****************************************/

int trace_level = TRACE_ERROR | TRACE_FATAL|TRACE_INFO;
module_param(trace_level, int , S_IRUGO);

int YWTRACE_Init(void)
{
    return 0;    
}


int YWTRACE_Print (const u32 level, const char * format, ...)
{
	va_list args;
	int r=0;
	if(trace_level & level)
	{
		va_start(args, format);
		if (level & (TRACE_ERROR | TRACE_FATAL))
		{
			printk("---------------");
		}
		r = vprintk(format, args);
		va_end(args);
	}
	return r;
}

//EXPORT_SYMBOL(YWTRACE_Init);
//EXPORT_SYMBOL(YWTRACE_Print);


/* ------------------------------- End of file ---------------------------- */





