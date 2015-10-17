#define LCD_8BIT_MODE   0	//����Һ������Ϊ8λģʽ   1Ϊʹ�� ���ݿ�ʹ�ø�8λDP_H
#define LCD_16BIT_MODE  1	//����Һ������Ϊ16λģʽ  1Ϊʹ��

#define LCD_DRIVER_ILI9320  0
#define LCD_DRIVER_ILI9325  0
#define LCD_DRIVER_ILI9328  0
#define LCD_DRIVER_ILI9331  0
#define LCD_DRIVER_R61509V  0
#define LCD_DRIVER_HX8352   1

#define X_MAX_PIXEL	        240
#define Y_MAX_PIXEL	        320

#if ((LCD_DRIVER_ILI9320)||(LCD_DRIVER_ILI9325)||(LCD_DRIVER_ILI9328)||(LCD_DRIVER_ILI9331))
//9320���ܼĴ�����ַ
#define WINDOW_XADDR_START		0x0050 // ˮƽ�Ŀ�ʼ��ַ��
#define WINDOW_XADDR_END		0x0051 // ˮƽ�Ľ�����ַ��
#define WINDOW_YADDR_START		0x0052 // ��ֱ�Ŀ�ʼ��ַ��
#define WINDOW_YADDR_END		0x0053 // ��ֱ�Ľ�����ַ��
#define GRAM_XADDR		    	0x0020 // GRAM ˮƽ�ĵ�ַ��
#define GRAM_YADDR		    	0x0021 // GRAM ��ֱ�ĵ�ַ��
#define GRAMWR 			    	0x0022 // GRAM
#endif

#if (LCD_DRIVER_R61509V)
//9325���ܼĴ�����ַ
#define WINDOW_XADDR_START		0x0210 // ˮƽ�Ŀ�ʼ��ַ��
#define WINDOW_XADDR_END		0x0211 // ˮƽ�Ľ�����ַ��
#define WINDOW_YADDR_START		0x0212 // ��ֱ�Ŀ�ʼ��ַ��
#define WINDOW_YADDR_END		0x0213 // ��ֱ�Ľ�����ַ��
#define GRAM_XADDR		    	0x0200 // GRAM ˮƽ�ĵ�ַ��
#define GRAM_YADDR		    	0x0201 // GRAM ��ֱ�ĵ�ַ��
#define GRAMWR 			    	0x0202 // GRAM
#endif

#if (LCD_DRIVER_HX8352)
#define WINDOW_XADDR_START_H	0x0002 // ˮƽ�Ŀ�ʼ��ַ��
#define WINDOW_XADDR_START_L	0x0003 // ˮƽ�Ŀ�ʼ��ַ��
#define WINDOW_XADDR_END_H		0x0004 // ˮƽ�Ľ�����ַ��
#define WINDOW_XADDR_END_L		0x0005 // ˮƽ�Ľ�����ַ��

#define WINDOW_YADDR_START_H	0x0006 // ��ֱ�Ŀ�ʼ��ַ��
#define WINDOW_YADDR_START_L	0x0007 // ��ֱ�Ŀ�ʼ��ַ��
#define WINDOW_YADDR_END_H		0x0008 // ��ֱ�Ľ�����ַ��
#define WINDOW_YADDR_END_L		0x0009 // ��ֱ�Ľ�����ַ��

#define GRAM_XADDR_H		    0x0002 // GRAM ˮƽ�ĵ�ַ��
#define GRAM_XADDR_L		    0x0003 // GRAM ˮƽ�ĵ�ַ��
#define GRAM_YADDR_H		    0x0004 // GRAM ��ֱ�ĵ�ַ��
#define GRAM_YADDR_L		    0x0005 // GRAM ��ֱ�ĵ�ַ��
#define GRAMWR 			    	0x0022 // GRAM
#endif

