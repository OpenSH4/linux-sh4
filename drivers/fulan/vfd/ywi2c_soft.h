/**********************************�ļ�ͷ��ע��************************************/
//
//
//                      Copyright (C), 2005-2010, AV Frontier Tech. Co., Ltd.
//
//
// �ļ�����      ywi2c_soft.h
//
// �����ߣ�      chenj
//
// ����ʱ�䣺    2011.02.10
//
// �ļ�������    ģ��I2C����
//
// �޸ļ�¼��   ��       ��      ��      ��       �汾      �޶�
//              ---------        ---------        -----     -----
//              2011.02.10       chenj            0.01      �½�
/*****************************************************************************************/

#ifndef __YWI2C_SOFT_H__
#define __YWI2C_SOFT_H__


#ifdef __cplusplus
extern "C" {
#endif


typedef U32     YWI2CSoft_Handle_t;



typedef struct YWI2cSoft_InitParam_s
{

    BOOL    IsSlaveDevice;

    U32     SCLPioIndex;
    U32     SDAPioIndex;

    U32     Speed;
}YWI2cSoft_InitParam_t;



typedef struct YWI2cSoft_OpenParams_s
{
    U16              I2cAddress;

} YWI2cSoft_OpenParams_t;



/* C++ support */
/* ----------- */
#ifdef __cplusplus
}
#endif




#endif

