/**********************************�ļ�ͷ��ע��************************************/
//
//
//                      Copyright (C), 2005-2010, AV Frontier Tech. Co., Ltd.
//
//
// �ļ�����     ywgpio_def.h
//
// �����ߣ�     CS
//
// ����ʱ�䣺   2008.04.23
//
// �ļ�������   GPIO�ڹ��ܶ����ͷ�ļ���
//
// �޸ļ�¼��   ��       ��      ��      ��       �汾      �޶�
//              ---------         ---------       -----     -----
//             2008.04.23         CS              0.01       �½�
//
/*****************************************************************************************/


#ifndef __YWGPIO_DEF_H
#define __YWGPIO_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/*
#define PIO_BIT_0    1
#define PIO_BIT_1    2
#define PIO_BIT_2    4
#define PIO_BIT_3    8
#define PIO_BIT_4    16
#define PIO_BIT_5    32
#define PIO_BIT_6    64
#define PIO_BIT_7    128
*/

#define        PIO_BITS                8


#ifdef CONFIG_CPU_SUBTYPE_STB7100
/******************************  PIO_0 �����÷�����*********************************/

#define GPIO_SC0UART_TXD_PORT              0                /*SMART0 Uart Data TXD IO Port*/
#define GPIO_SC0UART_TXD_BIT               0

#define GPIO_SC0UART_RXD_PORT              0                /*SMART0 Uart Data RXD IO Port*/
#define GPIO_SC0UART_RXD_BIT               1

//cs added for NEW Grass Board.20091120
#ifdef GMOON_IBOX_VER1_0
#define GPIO_SMART_RESET_PORT              (0)              /*SMART CARD RESET*/
#define GPIO_SMART_RESET_BIT               (2)              /*���ڰ���CA ��λ �� 0 ��Ч*/

#define GPIO_CA_SW_PORT                    (0)              /*CA �����л� ��1 Ϊ�ⲿ���ܿ���0 Ϊ�ڲ�����CA*/
#define GPIO_CA_SW_BIT                     (6)
#endif

#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_VCC_EN3_PORT                  0                /*CI��������*/
#define GPIO_VCC_EN3_BIT                   2
#endif


//end add

#define GPIO_SC0_CLK_PORT                  0                /*SMART0 CLK IO Port*/
#define GPIO_SC0_CLK_BIT                   3

#define GPIO_SC0_RST_PORT                  0                /*SMART0 RST IO Port*/
#define GPIO_SC0_RST_BIT                   4

#define GPIO_SC0_VCC_PORT                  0                /*SMART0 VCC IO Port*/
#define GPIO_SC0_VCC_BIT                   5

#define GPIO_SC0_DETECT_PORT               0                /*SMART0 Detect IO Port*/
#define GPIO_SC0_DETECT_BIT                7



/******************************  PIO_1 �����÷�����*********************************/

#define GPIO_SC1UART_TXD_PORT              1                /*SMART1 Uart Data TXD IO Port*/
#define GPIO_SC1UART_TXD_BIT               0

#define GPIO_SC1UART_RXD_PORT              1                /*SMART1 Uart Data RXD IO Port*/
#define GPIO_SC1UART_RXD_BIT               1

//cs added for NEW Grass Board.20091120
#ifdef GMOON_IBOX_VER1_0
#define GPIO_CI_DETECT_PORT                (1)              /*CI_DET*/
#define GPIO_CI_DETECT_BIT                 (2)
#endif

#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_TUNER_FIRST_LNB13_18_PORT           1                /*TUNER��LNB 14/19�������ڵ�IO  ��*/
#define GPIO_TUNER_FIRST_LNB13_18_BIT            2
#endif


#define GPIO_SC1_CLK_PORT                  1                /*SMART1 CLK IO Port*/
#define GPIO_SC1_CLK_BIT                   3

#define GPIO_SC1_RST_PORT                  1                /*SMART1 RST IO Port*/
#define GPIO_SC1_RST_BIT                   4

#define GPIO_SC1_VCC_PORT                  1                /*SMART1 VCC IO Port*/
#define GPIO_SC1_VCC_BIT                   5

//cs added for NEW Grass Board.20091120
#ifdef GMOON_IBOX_VER1_0
#define GPIO_CI_PASS_PORT                  (1)              /*Ci pass��IO  ��*/
#define GPIO_CI_PASS_BIT                   (6)
#endif

#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_TUNER_FIRST_LNB_POWER_PORT                1                /*SET LNB_POWER ���ڵ�PIO ��*/
#define GPIO_TUNER_FIRST_LNB_POWER_BIT                 6
#endif


#define GPIO_SC1_DETECT_PORT               1                /*SMART1 Detect IO Port*/
#define GPIO_SC1_DETECT_BIT                7



/******************************  PIO_2 �����÷�����*********************************/

#define GPIO_I2C_FIRST_SCL_PORT            2                       /*I2C[0] ��CLK IO  ��*/
#define GPIO_I2C_FIRST_SCL_BIT             0

#define GPIO_I2C_FIRST_SDA_PORT            2                       /*I2C[0] ��Data IO  ��*/
#define GPIO_I2C_FIRST_SDA_BIT             1

/*CS added for AVOutput and other PIO Ctrl. 20091013*/
#ifdef USE_HC595
#define GPIO_HC595_SRCK0_PORT              (2)
#define GPIO_HC595_SRCK0_BIT               (2)

#define GPIO_HC595_RCK0_PORT               (2)
#define GPIO_HC595_RCK0_BIT                (3)

#define GPIO_HC595_DS0_PORT                (2)
#define GPIO_HC595_DS0_BIT                 (4)

#define GPIO_HC595_SRCK1_PORT              (2)
#define GPIO_HC595_SRCK1_BIT               (5)

#define GPIO_HC595_RCK1_PORT               (2)
#define GPIO_HC595_RCK1_BIT                (6)

#define GPIO_HC595_DS1_PORT                (2)
#define GPIO_HC595_DS1_BIT                 (7)

#endif

#ifdef USE_STV6418

#define GPIO_TUNER_SECOND_RESET_PORT       2                       /*CAB��λ���ڵ�IO  ��*/
#define GPIO_TUNER_SECOND_RESET_BIT        2

#define GPIO_TUNER_FIRST_RESET_PORT        2                       /*TUNER��λ���ڵ�IO  ��*/
#define GPIO_TUNER_FIRST_RESET_BIT         3

#define GPIO_TUNER_LNB14_19_PORT           2                       /*TUNER��LNB 14/19�������ڵ�IO  ��*/
#define GPIO_TUNER_LNB14_19_BIT            4

#define GPIO_CI_RESET_PORT                 2                       /*CI_RESET*/
#define GPIO_CI_RESET_BIT                  5

#define GPIO_DISEQC_IORX_PORT              2                       /*DISEQC_IORX���ڵ�IO  ��*/
#define GPIO_DISEQC_IORX_BIT               6

#define GPIO_HDD_VCC_PORT                  2                       /* HDD VCC Enable.*/
#define GPIO_HDD_VCC_BIT                   7
#endif
/*End add.*/

/******************************  PIO_3 �����÷�����*********************************/

#define GPIO_I2C_SECOND_SCL_PORT           3                       /*I2C[1] ��CLK IO  ��*/
#define GPIO_I2C_SECOND_SCL_BIT            0

#define GPIO_I2C_SECOND_SDA_PORT           3                       /*I2C[1] ��Data IO  ��*/
#define GPIO_I2C_SECOND_SDA_BIT            1

#if 0
#define GPIO_UART_DATA_TXD_PORT            3                       /*����TXD ���ڵ�IO  ��*/
#define GPIO_UART_DATA_TXD_BIT             0

#define GPIO_UART_DATA_RXD_PORT            3                       /*����RXD ���ڵ�IO  ��*/
#define GPIO_UART_DATA_RXD_BIT             1
#endif

//cs added for NEW Grass Board.20091120
#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_CI_PASS_PORT                  3                       /*Ci pass��IO  ��*/
#define GPIO_CI_PASS_BIT                   2
#endif


#define GPIO_BLAST_RXD_PORT                3                       /*irʹ�õ�IO��*/
#define GPIO_BLAST_RXD_BIT                 3

#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_CI_DETECT_PORT                3                       /*CI_DET*/
#define GPIO_CI_DETECT_BIT                 4

#define GPIO_CI_READY_PORT                 3                       /*CI_READY*/
#define GPIO_CI_READY_BIT                  5
#endif


#define GPIO_PHY_RESET_PORT                3                       /*PHY Reset �� IO  ��*/
#define GPIO_PHY_RESET_BIT                 6

/******************************  PIO_4 �����÷�����*********************************/

#define GPIO_I2C_THIRD_SCL_PORT           4                       /*I2C[2] ��CLK IO  ��*/
#define GPIO_I2C_THIRD_SCL_BIT            0

#define GPIO_I2C_THIRD_SDA_PORT           4                       /*I2C[2] ��Data IO  ��*/
#define GPIO_I2C_THIRD_SDA_BIT            1


//cs added for NEW Grass Board.20091120
#ifdef GMOON_IBOX_VER1_0
#define GPIO_UART0_DATA_RXD_PORT         (4)                      /*����0 RXD ���ڵ�IO  ��*/
#define GPIO_UART0_DATA_RXD_BIT          (2)

#define GPIO_UART0_DATA_TXD_PORT         (4)                      /*����0 TXD ���ڵ�IO  ��*/
#define GPIO_UART0_DATA_TXD_BIT          (3)

#define GPIO_UART0_CTSD_PORT             (4)                      /*����0 CTSD ���ڵ�IO  ��*/
#define GPIO_UART0_CTSD_BIT              (4)                      /*NOT USED*/

#define GPIO_UART0_RTSD_PORT             (4)                      /*����0 RTSD ���ڵ�IO  ��*/
#define GPIO_UART0_RTSD_BIT              (5)                      /*NOT USED*/
#endif

#if defined(GRASS_IBOXHD9_VER1_1) || defined(MOON_IBOX_VER1_1)
#define GPIO_UART_DATA_RXD_PORT          (4)                       /*����RXD ���ڵ�IO  ��*/
#define GPIO_UART_DATA_RXD_BIT           (2)

#define GPIO_UART_DATA_TXD_PORT          (4)                       /*����TXD ���ڵ�IO  ��*/
#define GPIO_UART_DATA_TXD_BIT           (3)

#define GPIO_TUNER_SET12V_PORT           (4)                       /*12V �����IO  ��*/
#define GPIO_TUNER_SET12V_BIT            (6)
#endif

//end add

#define GPIO_HDMI_HPD_PORT                4                       /*HDMI ����*/
#define GPIO_HDMI_HPD_BIT                 7

/******************************  PIO_5 �����÷�����*********************************/
//cs added for NEW Grass Board.20091120
#ifdef GMOON_IBOX_VER1_0
#define GPIO_UART1_DATA_TXD_PORT         (5)                      /*����1 TXD ���ڵ�IO  ��*/
#define GPIO_UART1_DATA_TXD_BIT          (0)

#define GPIO_UART1_DATA_RXD_PORT         (5)                      /*����1 RXD ���ڵ�IO  ��*/
#define GPIO_UART1_DATA_RXD_BIT          (1)

#define GPIO_UART1_CTSD_PORT             (5)                      /*����1 CTSD ���ڵ�IO  ��*/
#define GPIO_UART1_CTSD_BIT              (2)                      /*NOT USED*/

#define GPIO_UART1_RTSD_PORT             (5)                      /*����1 RTSD ���ڵ�IO  ��*/
#define GPIO_UART1_RTSD_BIT              (3)                      /*NOT USED*/

#define GPIO_HDMI_CEC_PORT               (5)                      /*HDMI CTRL ���ڵ�IO  ��*/
#define GPIO_HDMI_CEC_BIT                (5)

#define GPIO_USB_EN_PORT                 (5)                      /*USB ��Դʹ�ܣ�1 ��Ч*/
#define GPIO_USB_EN_BIT                  (6)

#define GPIO_USB_PWR_PORT                (5)                      /*USB ���ؼ�⣬0 ��Ч*/
#define GPIO_USB_PWR_BIT                 (7)
#endif

/*CS added for AVOutput and other PIO Ctrl. 20091013*/
#ifdef USE_HC595
#define GPIO_VCR_DET_PORT                (5)                      /*VCR DET ���ڵ�IO  ��*/
#define GPIO_VCR_DET_BIT                 (4)
#endif
/*end add*/

#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_TUNER_SWITCH_PORT             5                       /*TUNER SWITCH��IO  ��*/
#define GPIO_TUNER_SWITCH_BIT              6
#endif
//end add







/******************************  PIO_6 �����÷�����*********************************/



/*********************************************************************************************/


/******************************  PIO_ALL INDEX �÷�����*********************************/

//cs added for NEW Grass Board.20091120
#ifdef GMOON_IBOX_VER1_0
#define GPIO_SC0UART_RXD_INDEX              ( GPIO_SC0UART_RXD_PORT*PIO_BITS + GPIO_SC0UART_RXD_BIT )
#define GPIO_SC0UART_TXD_INDEX              ( GPIO_SC0UART_TXD_PORT*PIO_BITS + GPIO_SC0UART_TXD_BIT )
#define GPIO_SC1UART_RXD_INDEX              ( GPIO_SC1UART_RXD_PORT*PIO_BITS + GPIO_SC1UART_RXD_BIT )
#define GPIO_SC1UART_TXD_INDEX              ( GPIO_SC1UART_TXD_PORT*PIO_BITS + GPIO_SC1UART_TXD_BIT )
#define GPIO_PHY_RESET_INDEX                ( GPIO_PHY_RESET_PORT *PIO_BITS + GPIO_PHY_RESET_BIT)
#define GPIO_CA_SW_INDEX                    ( GPIO_CA_SW_PORT*PIO_BITS + GPIO_CA_SW_BIT)
#endif

#ifdef GRASS_IBOXHD9_VER1_1
#define GPIO_TUNER_FIRST_LNB13_18_INDEX           ( GPIO_TUNER_FIRST_LNB13_18_PORT*PIO_BITS + GPIO_TUNER_FIRST_LNB13_18_BIT )
#define GPIO_TUNER_FIRST_LNB_POWER_INDEX          ( GPIO_TUNER_FIRST_LNB_POWER_PORT*PIO_BITS + GPIO_TUNER_FIRST_LNB_POWER_BIT )
#define GPIO_TUNER_SWITCH_INDEX             ( GPIO_TUNER_SWITCH_PORT*PIO_BITS + GPIO_TUNER_SWITCH_BIT )
#define GPIO_VCC_EN3_INDEX                  ( GPIO_VCC_EN3_PORT*PIO_BITS + GPIO_VCC_EN3_BIT)
#define GPIO_CI_DETECT_INDEX                ( GPIO_CI_DETECT_PORT*PIO_BITS + GPIO_CI_DETECT_BIT)
#define GPIO_CI_READY_INDEX                 ( GPIO_CI_READY_PORT*PIO_BITS + GPIO_CI_READY_BIT)
#endif

#define GPIO_CI_PASS_INDEX                  ( GPIO_CI_PASS_PORT*PIO_BITS + GPIO_CI_PASS_BIT )

#if defined(GRASS_IBOXHD9_VER1_1) || defined(MOON_IBOX_VER1_1)
#define GPIO_TUNER_SET_12V_INDEX            ( GPIO_TUNER_SET12V_PORT*PIO_BITS + GPIO_TUNER_SET12V_BIT )
#endif

/*CS added for AVOutput and other PIO Ctrl. 20091013*/

#ifdef USE_STV6418
#define GPIO_TUNER_FIRST_LNB14_19_INDEX           ( GPIO_TUNER_LNB14_19_PORT *PIO_BITS + GPIO_TUNER_LNB14_19_BIT)
#define GPIO_CI_RESET_INDEX                 ( GPIO_CI_RESET_PORT*PIO_BITS + GPIO_CI_RESET_BIT)
#define GPIO_HDD_VCCEN_INDEX                ( GPIO_HDD_VCC_PORT *PIO_BITS + GPIO_HDD_VCC_BIT)
#define GPIO_TUNER_FIRST_RESET_INDEX        ( GPIO_TUNER_FIRST_RESET_PORT*PIO_BITS + GPIO_TUNER_FIRST_RESET_BIT )
#define GPIO_TUNER_SECOND_RESET_INDEX       ( GPIO_TUNER_SECOND_RESET_PORT*PIO_BITS + GPIO_TUNER_SECOND_RESET_BIT )
#define GPIO_DISEQC_IORX_INDEX              ( GPIO_DISEQC_IORX_PORT*PIO_BITS + GPIO_DISEQC_IORX_BIT )
#endif

#ifdef USE_HC595
#define GPIO_HC595_SRCK0_INDEX              ( GPIO_HC595_SRCK0_PORT *PIO_BITS + GPIO_HC595_SRCK0_BIT)
#define GPIO_HC595_RCK0_INDEX               ( GPIO_HC595_RCK0_PORT*PIO_BITS + GPIO_HC595_RCK0_BIT)
#define GPIO_HC595_DS0_INDEX                ( GPIO_HC595_DS0_PORT*PIO_BITS + GPIO_HC595_DS0_BIT)
#define GPIO_HC595_SRCK1_INDEX              ( GPIO_HC595_SRCK1_PORT*PIO_BITS + GPIO_HC595_SRCK1_BIT)
#define GPIO_HC595_RCK1_INDEX               ( GPIO_HC595_RCK1_PORT*PIO_BITS + GPIO_HC595_RCK1_BIT)
#define GPIO_HC595_DS1_INDEX                ( GPIO_HC595_DS1_PORT*PIO_BITS + GPIO_HC595_DS1_BIT)
#define GPIO_VCT_IN_DCT_INDEX               ( GPIO_VCR_DET_PORT*PIO_BITS + GPIO_VCR_DET_BIT)
#endif
/*End add*/
#endif




/**********************************************7111***********************************/
/*************************************************************************************/
/*************************************************************************************/
/*************************************************************************************/
#ifdef CONFIG_CPU_SUBTYPE_STX7111
#define YW_HW_SAT7111_220
#ifdef YW_HW_SAT7111_220
/******************************  PIO_0 �����÷�����*********************************/
#define GPIO_SC0UART_TXD_PORT              0                /*SMART0 Uart Data TXD IO Port*/
#define GPIO_SC0UART_TXD_BIT               0

#define GPIO_SC0UART_RXD_PORT              0                /*SMART0 Uart Data RXD IO Port*/
#define GPIO_SC0UART_RXD_BIT               1

#define GPIO_SC0_CLK_PORT                  0                /*SMART0 CLK IO Port*/
#define GPIO_SC0_CLK_BIT                   3

#define GPIO_SC0_RST_PORT                  0                /*SMART0 CARD RESET*/
#define GPIO_SC0_RST_BIT                   4                /*���ڰ���CA ��λ �� 0 ��Ч*/

#define GPIO_SC0_VCC_PORT                  0                /*SMART0 VCC IO Port*/
#define GPIO_SC0_VCC_BIT                   5

#define GPIO_SC0_DETECT_PORT               0                /*SMART0 Detect IO Port*/
#define GPIO_SC0_DETECT_BIT                7

/******************************  PIO_1 �����÷�����*********************************/
#define GPIO_SC1UART_TXD_PORT              1                /*SMART1 Uart Data TXD IO Port*/
#define GPIO_SC1UART_TXD_BIT               0

#define GPIO_SC1UART_RXD_PORT              1                /*SMART1 Uart Data RXD IO Port*/
#define GPIO_SC1UART_RXD_BIT               1

#define GPIO_SC1_CLK_PORT                  1                /*SMART1 CLK IO Port*/
#define GPIO_SC1_CLK_BIT                   3

#define GPIO_SC1_RST_PORT                  1                /*SMART1 CARD RESET*/
#define GPIO_SC1_RST_BIT                   4                /*���ڰ���CA ��λ �� 0 ��Ч*/

#define GPIO_SC1_VCC_PORT                  1                /*SMART1 VCC IO Port*/
#define GPIO_SC1_VCC_BIT                   5

#define GPIO_SC1_DETECT_PORT               1                /*SMART1 Detect IO Port�����ýţ���hdmi cec�Ÿ���*/
#define GPIO_SC1_DETECT_BIT                7




#define GPIO_TUNER_SECOND_LNB14_19_PORT   (1)               /*PIO1.6-- ���� LNB 14/19����(Tuner1)*/
#define GPIO_TUNER_SECOND_LNB14_19_BIT    (6)

#define GPIO_HDMI_CEC_PORT                (1)               /*PIO1.7 HDMI CTRL���ڵ�IO��*/
#define GPIO_HDMI_CEC_BIT                 (7)



/******************************  PIO_2 �����÷�����*********************************/
#define GPIO_TUNER_SET12V_PORT           (2)                       /*12V �����IO ��*/
#define GPIO_TUNER_SET12V_BIT            (3)

#define GPIO_AUDIO_MUTE_PORT	         (2)                       /*PIO2.4 ---��������*/
#define GPIO_AUDIO_MUTE_BIT	             (4)

#define GPIO_ESAM_RESET_PORT	         (2)                       /*PIO2.5 ---ESAM����IC��λ��*/
#define GPIO_ESAM_RESET_BIT	             (5)

#define GPIO_CA_ESAM_SW_PORT	         (2)                       /*PIO2.6 ---CA��ESAM����IC���л���*/
#define GPIO_CA_ESAM_SW_BIT	             (6)

#if 1
#define GPIO_TUNER_SECOND_RESET_PORT     (2)                      /*����2�ĸ�λ����*/
#define GPIO_TUNER_SECOND_RESET_BIT      (7)
#endif

/******************************  PIO_3 �����÷�����*********************************/

#define GPIO_I2C_FIRST_SCL_PORT           3                       /*I2C[0] ��CLK IO  ��*/
#define GPIO_I2C_FIRST_SCL_BIT            0

#define GPIO_I2C_FIRST_SDA_PORT           3                       /*I2C[0] ��Data IO  ��*/
#define GPIO_I2C_FIRST_SDA_BIT            1

#define GPIO_BLAST_RXD_PORT               3                       /*irʹ�õ�IO��*/
#define GPIO_BLAST_RXD_BIT                3



/******************************  PIO_4 �����÷�����*********************************/
#define GPIO_I2C_SECOND_SCL_PORT         (4)                      /*I2C[1] ��CLK IO  ��*/
#define GPIO_I2C_SECOND_SCL_BIT          (0)

#define GPIO_I2C_SECOND_SDA_PORT         (4)                      /*I2C[1] ��Data IO  ��*/
#define GPIO_I2C_SECOND_SDA_BIT          (1)

#define GPIO_UART0_DATA_RXD_PORT         (4)                      /*232���� RXD ���ڵ�IO  ��*/
#define GPIO_UART0_DATA_RXD_BIT          (2)

#define GPIO_UART0_DATA_TXD_PORT         (4)                      /*232���� TXD ���ڵ�IO  ��*/
#define GPIO_UART0_DATA_TXD_BIT          (3)

#define GPIO_UART0_CTSD_PORT             (4)                      /*232 ���� CTSD ���ڵ�IO  ��*/
#define GPIO_UART0_CTSD_BIT              (4)

#define GPIO_UART0_RTSD_PORT             (4)                      /*232 ���� RTSD ���ڵ�IO  ��*/
#define GPIO_UART0_RTSD_BIT              (5)

#define GPIO_TUNER_SECOND_LNB13_18_PORT  (4)                      /*PIO4.6-- ���� LNB 13/18����(Tuner1)*/
#define GPIO_TUNER_SECOND_LNB13_18_BIT   (6)

#define GPIO_HDMI_HPD_PORT               (4)                      /*HDMI�Ȳ�μ��*/
#define GPIO_HDMI_HPD_BIT                (7)


/******************************  PIO_5 �����÷�����*********************************/
#define GPIO_PHY_RESET_PORT              (5)                      /*PHY Reset �� IO  ��*/
#define GPIO_PHY_RESET_BIT               (0)

#define GPIO_TUNER_SECOND_LNB_POWER_PORT (5)                      /*��������2��LNB���ؽ�*/
#define GPIO_TUNER_SECOND_LNB_POWER_BIT  (2)

#if 0
#define GPIO_TUNER_SECOND_RESET_PORT     (5)                      /*����2�ĸ�λ����*/
#define GPIO_TUNER_SECOND_RESET_BIT      (3)
#endif
#define GPIO_HDMI_EN_PORT                (5)                      /*HDMIʹ�ܿ���*/
#define GPIO_HDMI_EN_BIT                 (4)

#if 0
#define GPIO_HDMI_PWR_PORT               (5)                      /*HDMI������������*/
#define GPIO_HDMI_PWR_BIT                (5)
#else
#define GPIO_TUNER_FIRST_LNB14_19_PORT   (5)                      /*PIO5.5-- ���� LNB 14/19����(Tuner0)*/
#define GPIO_TUNER_FIRST_LNB14_19_BIT    (5)
#endif

#define GPIO_USB_PWR_PORT                (5)                      /*USB ���ؼ�⣬0 ��Ч*/
#define GPIO_USB_PWR_BIT                 (6)

#define GPIO_USB_EN_PORT                 (5)                      /*USB ��Դʹ�ܣ�1 ��Ч*/
#define GPIO_USB_EN_BIT                  (7)


/******************************  PIO_6 �����÷�����*********************************/

#define  GPIO_CVBS_RGB_SW_PORT	        (6)				/* ����CVBS/RGB�л�*/
#define  GPIO_CVBS_RGB_SW_BIT	        (0)

#define  GPIO_ON_STBY_PORT	            (6)				/* TV/STA*/
#define  GPIO_ON_STBY_BIT	            (1)

#define  GPIO_SCART_169_43_PORT	        (6)				/* 16:9���ƽ�*/
#define  GPIO_SCART_169_43_BIT	        (2)

#if 1
#define GPIO_TUNER_FIRST_LNB_POWER_PORT (6)                 /*��������1��LNB���ؽ�*/
#define GPIO_TUNER_FIRST_LNB_POWER_BIT  (5)

#define GPIO_TUNER_FIRST_LNB13_18_PORT  (6)                 /*��������1�ļ����������*/
#define GPIO_TUNER_FIRST_LNB13_18_BIT   (6)
#else //before version my board
#define GPIO_TUNER_FIRST_LNB_POWER_PORT (1)                 /*��������1��LNB���ؽ�*/
#define GPIO_TUNER_FIRST_LNB_POWER_BIT  (2)

#define GPIO_TUNER_FIRST_LNB13_18_PORT  (0)                 /*��������1�ļ����������*/
#define GPIO_TUNER_FIRST_LNB13_18_BIT   (2)

#endif
#if 0
#define GPIO_SCART_VCR_DECT_PORT		   6
#define GPIO_SCART_VCR_DECT_BIT			   3
#endif
#define GPIO_TV_VCR_SWITCH_PORT			   6
#define GPIO_TV_VCR_SWITCH_BIT			   4
/*********************************************************************************************/
#define GPIO_I2C_THIRD_SCL_BIT            0
#define GPIO_I2C_THIRD_SDA_BIT            1

/******************************  PIO_ALL INDEX �÷�����*********************************/

#define GPIO_AUDIO_MUTE_INDEX                (GPIO_AUDIO_MUTE_PORT*PIO_BITS + GPIO_AUDIO_MUTE_BIT)

#define GPIO_TUNER_FIRST_LNB13_18_INDEX      (GPIO_TUNER_FIRST_LNB13_18_PORT*PIO_BITS + GPIO_TUNER_FIRST_LNB13_18_BIT )
#define GPIO_TUNER_SECOND_LNB13_18_INDEX     (GPIO_TUNER_SECOND_LNB13_18_PORT*PIO_BITS + GPIO_TUNER_SECOND_LNB13_18_BIT )

#define GPIO_TUNER_FIRST_LNB14_19_INDEX    ( GPIO_TUNER_FIRST_LNB14_19_PORT *PIO_BITS + GPIO_TUNER_FIRST_LNB14_19_BIT)
#define GPIO_TUNER_SECOND_LNB14_19_INDEX     ( GPIO_TUNER_SECOND_LNB14_19_PORT *PIO_BITS + GPIO_TUNER_SECOND_LNB14_19_BIT)

#define GPIO_TUNER_FIRST_LNB_POWER_INDEX     ( GPIO_TUNER_FIRST_LNB_POWER_PORT*PIO_BITS + GPIO_TUNER_FIRST_LNB_POWER_BIT )
#define GPIO_TUNER_SECOND_LNB_POWER_INDEX    ( GPIO_TUNER_SECOND_LNB_POWER_PORT*PIO_BITS + GPIO_TUNER_SECOND_LNB_POWER_BIT )

//#define GPIO_TUNER_FIRST_RESET_INDEX       ( GPIO_TUNER_FIRST_RESET_PORT*PIO_BITS + GPIO_TUNER_FIRST_RESET_BIT )
#define GPIO_TUNER_SECOND_RESET_INDEX        ( GPIO_TUNER_SECOND_RESET_PORT*PIO_BITS + GPIO_TUNER_SECOND_RESET_BIT )

#define GPIO_TUNER_SET_12V_INDEX             ( GPIO_TUNER_SET12V_PORT*PIO_BITS + GPIO_TUNER_SET12V_BIT )

#define GPIO_PHY_RESET_INDEX                 ( GPIO_PHY_RESET_PORT *PIO_BITS + GPIO_PHY_RESET_BIT)

//#define GPIO_DISEQC_IOTX_INDEX             ( GPIO_DISEQC_IOTX_PORT*PIO_BITS + GPIO_DISEQC_IOTX_BIT )

//#define GPIO_DISEQC_IORX_INDEX             ( GPIO_DISEQC_IORX_PORT*PIO_BITS + GPIO_DISEQC_IORX_BIT )





//#define GPIO_CI_PASS_INDEX                  ( GPIO_CI_PASS_PORT*PIO_BITS + GPIO_CI_PASS_BIT )

//#define GPIO_TUNER_SWITCH_INDEX            ( GPIO_TUNER_SWITCH_PORT*PIO_BITS + GPIO_TUNER_SWITCH_BIT )

//#define GPIO_SC0UART_RXD_INDEX              ( GPIO_SC0UART_RXD_PORT*PIO_BITS + GPIO_SC0UART_RXD_BIT )

//#define GPIO_SC0UART_TXD_INDEX              ( GPIO_SC0UART_TXD_PORT*PIO_BITS + GPIO_SC0UART_TXD_BIT )

//#define GPIO_SC1UART_RXD_INDEX              ( GPIO_SC1UART_RXD_PORT*PIO_BITS + GPIO_SC1UART_RXD_BIT )

//#define GPIO_SC1UART_TXD_INDEX              ( GPIO_SC1UART_TXD_PORT*PIO_BITS + GPIO_SC1UART_TXD_BIT )

//#define GPIO_BLAST_RXD_INDEX               ( GPIO_BLAST_RXD_PORT*PIO_BITS + GPIO_BLAST_RXD_BIT )

//#define GPIO_BLAST_TXD_INDEX               ( GPIO_BLAST_TXD_PORT*PIO_BITS + GPIO_BLAST_RXD_BIT )
//#define GPIO_HDD_VCCEN_INDEX                ( GPIO_HDD_VCC_PORT *PIO_BITS + GPIO_HDD_VCC_BIT)

//d48zm 2009.12.9 add
#define GPIO_CVBS_RGB_SWITCH_INDEX		   	(GPIO_CVBS_RGB_SW_PORT*PIO_BITS + GPIO_CVBS_RGB_SW_BIT)
#define GPIO_ON_STBY_INDEX                	(GPIO_ON_STBY_PORT*PIO_BITS + GPIO_ON_STBY_BIT)
#define GPIO_169_43_INDEX				   	(GPIO_SCART_169_43_PORT*PIO_BITS + GPIO_SCART_169_43_BIT)
#if 0
#define GPIO_SCART_VCR_DECT_INDEX		    (GPIO_SCART_VCR_DECT_PORT*PIO_BITS + GPIO_SCART_VCR_DECT_BIT)
#define GPIO_TV_VCR_SWITCH_INDEX		    (GPIO_TV_VCR_SWITCH_PORT*PIO_BITS + GPIO_TV_VCR_SWITCH_BIT)
#endif
//d48zm add end

#endif
#endif

#ifdef CONFIG_CPU_SUBTYPE_STX7105


/******************************  PIO_0 �����÷�����*********************************/
#define GPIO_SC0UART_TXD_PORT              0                /*SMART0 Uart Data TXD IO Port*/
#define GPIO_SC0UART_TXD_BIT               0

#define GPIO_SC0UART_RXD_PORT              0                /*SMART0 Uart Data RXD IO Port*/
#define GPIO_SC0UART_RXD_BIT               1

#define GPIO_SC0_CLK_PORT                  0                /*SMART0 CLK IO Port*/
#define GPIO_SC0_CLK_BIT                   3

#define GPIO_SC0_RST_PORT                  0                /*SMART0 RST IO Port*/
#define GPIO_SC0_RST_BIT                   4

#define GPIO_SC0_VCC_PORT                  0                /*SMART0 VCC IO Port*/
#define GPIO_SC0_VCC_BIT                   5

#define GPIO_SC0_DETECT_PORT               0                /*SMART0 Detect IO Port*/
#define GPIO_SC0_DETECT_BIT                7

/******************************  PIO_1 �����÷�����*********************************/

#define GPIO_SC1UART_TXD_PORT              1                /*SMART1 Uart Data TXD IO Port*/
#define GPIO_SC1UART_TXD_BIT               0

#define GPIO_SC1UART_RXD_PORT              1                /*SMART1 Uart Data RXD IO Port*/
#define GPIO_SC1UART_RXD_BIT               1

//#define GPIO_SC1_CLKGenExtClk_PORT         1                /*SMART1 CLKGenExtCLK Port*/
//#define GPIO_SC1_CLKGenExtClk_BIT          2

#define GPIO_SC1_CLK_PORT                  1                /*SMART1 CLK IO Port*/
#define GPIO_SC1_CLK_BIT                   3

#define GPIO_SC1_RST_PORT                  1                /*SMART1 RST IO Port*/
#define GPIO_SC1_RST_BIT                   4

#define GPIO_SC1_VCC_PORT                  1                /*SMART1 VCC IO Port*/
#define GPIO_SC1_VCC_BIT                   5

#define GPIO_SC1_DETECT_PORT               1                /*SMART1 Detect IO Port*/
#define GPIO_SC1_DETECT_BIT                7

/******************************  PIO_2 �����÷�����*********************************/
#define GPIO_SIMULATE_I2C_SCL_PORT           2                    /*ģ��I2C��GPIO�� SCL*/
#define GPIO_SIMULATE_I2C_SCL_BIT            0

#define GPIO_SIMULATE_I2C_SDA_PORT           2                    /*ģ��I2C��GPIO�� SDA*/
#define GPIO_SIMULATE_I2C_SDA_BIT           1

#define GPIO_I2C_FIRST_SCL_PORT           2                       /*I2C[0] ��CLK IO  ��,used for hdmi auto detect*/
#define GPIO_I2C_FIRST_SCL_BIT            2

#define GPIO_I2C_FIRST_SDA_PORT           2                       /*I2C[0] ��Data IO  ��*/
#define GPIO_I2C_FIRST_SDA_BIT            3

#define GPIO_I2C_SECOND_SCL_PORT           2                       /*I2C[1] ��CLK IO  ��,used for S:tuner0*/
#define GPIO_I2C_SECOND_SCL_BIT            5

#define GPIO_I2C_SECOND_SDA_PORT           2                       /*I2C[1] ��Data IO  ��*/
#define GPIO_I2C_SECOND_SDA_BIT            6

/******************************  PIO_3 �����÷�����*********************************/
#define GPIO_BLAST_RXD_PORT               3                       /*irʹ�õ�IO�� RX pin*/
#define GPIO_BLAST_RXD_BIT                0

//#define GPIO_BLAST_TXD_PORT               3                       /*irʹ�õ�IO�� TX pin*/
//#define GPIO_BLAST_TXD_BIT                2

#define GPIO_TUNER_SECOND_RESET_PORT       3                     /*TUNER1��λ���ڵ�IO T/C:i2c2)*/
#define GPIO_TUNER_SECOND_RESET_BIT        3


#define GPIO_I2C_THIRD_SCL_PORT         (3)                      /*I2C[2] ��CLK IO  ��,used for tuner T/C*/
#define GPIO_I2C_THIRD_SCL_BIT          (4)

#define GPIO_I2C_THIRD_SDA_PORT         (3)                      /*I2C[2] ��Data IO  ��*/
#define GPIO_I2C_THIRD_SDA_BIT          (5)


#define GPIO_I2C_FOURTH_SCL_PORT        (3)                      /*I2C[3] ��CLK IO  �� used for S:tuner1*/
#define GPIO_I2C_FOURTH_SCL_BIT         (6)

#define GPIO_I2C_FOURTH_SDA_PORT        (3)                      /*I2C[3] ��Data IO  ��*/
#define GPIO_I2C_FOURTH_SDA_BIT         (7)


/******************************  PIO_4 �����÷�����*********************************/
#define GPIO_UART0_DATA_RXD_PORT         (4)                      /*232���� RXD ���ڵ�IO  ��*/
#define GPIO_UART0_DATA_RXD_BIT          (0)

#define GPIO_UART0_DATA_TXD_PORT         (4)                      /*232���� TXD ���ڵ�IO  ��*/
#define GPIO_UART0_DATA_TXD_BIT          (1)

#define GPIO_USB0_OC_PORT                (4)                      /*USB0 ���ؼ�⣬0 ��Ч*/
#define GPIO_USB0_OC_BIT                 (4)

#define GPIO_USB0_PWR_PORT                 (4)                      /*USB0 ��Դʹ�ܣ�1 ��Ч*/
#define GPIO_USB0_PWR_BIT                  (5)

#define GPIO_USB1_OC_PORT                (4)                      /*USB1 ���ؼ�⣬0 ��Ч*/
#define GPIO_USB1_OC_BIT                 (6)

#define GPIO_USB1_PWR_PORT                 (4)                      /*USB1 ��Դʹ�ܣ�1 ��Ч*/
#define GPIO_USB1_PWR_BIT                  (7)


/******************************  PIO_5 �����÷�����*********************************/
#define GPIO_TUNER_FIRST_RESET_PORT        5                     /*TUNER0��λ���ڵ�IO  ��:S:TUNER0(I2C1)*/
#define GPIO_TUNER_FIRST_RESET_BIT         5

#define GPIO_TUNER_THIRD_RESET_PORT        5                     /*TUNER2��λ���ڵ�IO  ��:S:TUNER1(I2C3)*/
#define GPIO_TUNER_THIRD_RESET_BIT         6

#define GPIO_PHY_RESET_PORT                5                       /*PHY Reset �� IO  ��*/
#define GPIO_PHY_RESET_BIT                 7


/******************************  PIO_9 �����÷�����*********************************/
#define GPIO_HDMI_HPD_PORT                9                      /*HDMI ����*/
#define GPIO_HDMI_HPD_BIT                 7


/******************************* PIO11  ���Ŷ��� ************************************/
#define GPIO_AUDIO_MUTE_PORT	         (11)                       /*PIO11.2 ---��������*/
#define GPIO_AUDIO_MUTE_BIT	             (2)

#define  GPIO_ON_STBY_PORT	            (11)				/* TV/STA*/
#define  GPIO_ON_STBY_BIT	            (3)

#define  GPIO_SCART_169_43_PORT	        (11)				/* 16:9���ƽ�*/
#define  GPIO_SCART_169_43_BIT	        (4)

#define  GPIO_CVBS_RGB_SW_PORT	        (11)				/* ����CVBS/RGB�л�*/
#define  GPIO_CVBS_RGB_SW_BIT	        (5)

#if 0
#define GPIO_TUNER_SET12V_PORT           (11)               /*12V ����� IO  ��*/ //һ��PIO11.7�������⣬�Ե�tuner��i2c����
#define GPIO_TUNER_SET12V_BIT            (6) //lwj change 7 to 6 for demo board
#else
#define GPIO_TUNER_SET12V_PORT           (11)               /*12V ����� IO  ��*/ //һ��PIO11.7�������⣬�Ե�tuner��i2c����
#define GPIO_TUNER_SET12V_BIT            (7)

#endif

/******************************* PIO15  ���Ŷ��� ************************************/
#if 0 //for demo board
#define GPIO_TUNER_SECOND_LNB14_19_PORT   (15)                      /*PIO15.4-- ���� LNB 14/19����(S:Tuner1)*/ //PIO15.4�ں��ù��ˣ����Ի������⡣����ĳ�������
#define GPIO_TUNER_SECOND_LNB14_19_BIT    (2) //lwj for demo board

#define GPIO_TUNER_SECOND_LNB_POWER_PORT   (15)                     /*PIO15.5-- ���� LNB���ؽ�(S:Tuner1)*/
#define GPIO_TUNER_SECOND_LNB_POWER_BIT     (3)//lwj for demo board

#define GPIO_TUNER_FIRST_LNB14_19_PORT   (15)                      /*PIO15.6-- ���� LNB 14/19����(S:Tuner0)*/
#define GPIO_TUNER_FIRST_LNB14_19_BIT    (6)

#define GPIO_TUNER_FIRST_LNB_POWER_PORT   (15)                     /*PIO15.7-- ���� LNB���ؽ�(S:Tuner0)*/
#define GPIO_TUNER_FIRST_LNB_POWER_BIT     (7)
#else
#define GPIO_TUNER_SECOND_LNB14_19_PORT   (15)                      /*PIO15.4-- ���� LNB 14/19����(S:Tuner1)*/ //PIO15.4�ں��ù��ˣ����Ի������⡣����ĳ�������
#define GPIO_TUNER_SECOND_LNB14_19_BIT    (4)

#define GPIO_TUNER_SECOND_LNB_POWER_PORT   (15)                     /*PIO15.5-- ���� LNB���ؽ�(S:Tuner1)*/
#define GPIO_TUNER_SECOND_LNB_POWER_BIT     (5)

#define GPIO_TUNER_FIRST_LNB14_19_PORT   (15)                      /*PIO15.6-- ���� LNB 14/19����(S:Tuner0)*/
#define GPIO_TUNER_FIRST_LNB14_19_BIT    (6)

#define GPIO_TUNER_FIRST_LNB_POWER_PORT   (15)                     /*PIO15.7-- ���� LNB���ؽ�(S:Tuner0)*/
#define GPIO_TUNER_FIRST_LNB_POWER_BIT     (7)

#endif
/******************************* PIO15  ���Ŷ��� end************************************/



#define GPIO_TUNER_FIRST_RESET_INDEX       ( GPIO_TUNER_FIRST_RESET_PORT*PIO_BITS + GPIO_TUNER_FIRST_RESET_BIT )
#define GPIO_TUNER_SECOND_RESET_INDEX      ( GPIO_TUNER_SECOND_RESET_PORT*PIO_BITS + GPIO_TUNER_SECOND_RESET_BIT )
#define GPIO_TUNER_THIRD_RESET_INDEX       ( GPIO_TUNER_THIRD_RESET_PORT*PIO_BITS + GPIO_TUNER_THIRD_RESET_BIT )


#define GPIO_TUNER_FIRST_LNB14_19_INDEX    ( GPIO_TUNER_FIRST_LNB14_19_PORT *PIO_BITS + GPIO_TUNER_FIRST_LNB14_19_BIT)
#define GPIO_TUNER_SECOND_LNB14_19_INDEX     ( GPIO_TUNER_SECOND_LNB14_19_PORT *PIO_BITS + GPIO_TUNER_SECOND_LNB14_19_BIT)

#define GPIO_TUNER_FIRST_LNB_POWER_INDEX     ( GPIO_TUNER_FIRST_LNB_POWER_PORT*PIO_BITS + GPIO_TUNER_FIRST_LNB_POWER_BIT )
#define GPIO_TUNER_SECOND_LNB_POWER_INDEX    ( GPIO_TUNER_SECOND_LNB_POWER_PORT*PIO_BITS + GPIO_TUNER_SECOND_LNB_POWER_BIT )

#define GPIO_TUNER_SET_12V_INDEX             ( GPIO_TUNER_SET12V_PORT*PIO_BITS + GPIO_TUNER_SET12V_BIT )

#define GPIO_PHY_RESET_INDEX                 ( GPIO_PHY_RESET_PORT *PIO_BITS + GPIO_PHY_RESET_BIT)

#define GPIO_AUDIO_MUTE_INDEX                (GPIO_AUDIO_MUTE_PORT*PIO_BITS + GPIO_AUDIO_MUTE_BIT)

#define GPIO_CVBS_RGB_SWITCH_INDEX		   	(GPIO_CVBS_RGB_SW_PORT*PIO_BITS + GPIO_CVBS_RGB_SW_BIT)
#define GPIO_ON_STBY_INDEX                	(GPIO_ON_STBY_PORT*PIO_BITS + GPIO_ON_STBY_BIT)
#define GPIO_169_43_INDEX				   	(GPIO_SCART_169_43_PORT*PIO_BITS + GPIO_SCART_169_43_BIT)

#endif



S8*  YWGPIOi_GetPortDeviceName(U32 PortIndex);

#ifdef __cplusplus
}
#endif

#endif


