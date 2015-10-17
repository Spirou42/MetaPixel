/*����˵��
STC12LE5A60S2 3.3V1T��Ƭ�� Ӳ��SPIֱ����������TM022HDH26Һ��
��Ƭ����Ƶ11.0592MHZ��ʵ�����Ƶ��Լ1��ˢ��һ��ȫ��
STC12C5A60S2 5V1T��Ƭ���ṹһ��,�����PDF����3.3V����
�������Ѿ����Կ��á�
�Ա���:pei_gang 2013/2/5
*/

#include <stc12.h> //STC12��Ƭ��ͷ�ļ�

//��ɫ����
#define Blue 		0x001f
#define Yellow		0xffe0
#define Green 	 	0x07e0
#define Black		0x0000
#define White		0xffff
#define Red			0xf800

//���Ŷ���
sbit	R_ESET =	P1^2; //Һ��RESET����,�ӵ�Ƭ��IO����
sbit	C_D	=	P1^3;//Һ��D/CX���ţ�����/������ƣ��ӵ�Ƭ��IO����
sbit	C_S	=	P1^4;//Һ��ƬѡCS,P1.4Ϊ��Ƭ��SPI���ߵ�SS����,ͨ��MSTRλ����ΪIOģʽ

//����3�����ŵ����ӡ���ӵ�Ƭ��Ĭ��Ӳ��SPI����,ֻ��Ҫ��SPCTL��ʹ�ܼ���,��������ж���
//sbit	SDI	=	P1^5; Һ��SDI����,����д��Һ��,�ӵ�Ƭ��MOSI����P1.5
//sbit	SDO	=	P1^6; Һ��SDO����,��Һ��������,�ӵ�Ƭ��MISO����P1.6
//sbit	SCL =   P1^7; Һ��SCLʱ������,�ӵ�Ƭ��SCLKʱ������P1.7

//LED����(10��)�ӵ�Դ3.3V,11-14Ϊ4��LED��������,��Ƭ��IO����,��0������
//3.3VʱÿLEDͨ������Լ20����,����Ҫ��������
sbit	LED1 =  P2^4;//����LED���ơ�
sbit	LED2 =  P2^5;//����LED����
sbit	LED3 =  P2^6;//����LED����
sbit	LED4 =  P2^7;//����LED����

/*SPCTL����  11010000=0xd0 �ò����µ�Ƭ��Ӳ��SPIΪ��ģʽ,P1.4ΪIOģʽ,SPIʱ��Ƶ��ΪMCUʱ��Ƶ��/4,Ϊ2.7648M
Bit7 SSIG  SS���ź��Կ���λ 1
Bit6 SPEN  SPIʹ��λ		1 
Bit5 DORD  ����˳��			0 ���λ���ȷ���
Bit4 MSTR  ��/��ģʽѡ��λ  1
Bit3 SPOL  ʱ�Ӽ���			0
Bit2 SPHA  ʱ����λ			0
Bit1 SPR1  ʱ������ѡ��		0 
Bit0 SPR0  ʱ������ѡ��		0
*/

//����
void Delay_us(int value)////��ʱ����_us
	{
	while (value)
	value--;
	}

void Delay_ms(int value)////��ʱ����_ms
	{
	while (value){
		Delay_us(999);
		value--;
		}
	}

void HD_reset(){
	//SPI��ʼ��
	SPDAT = 0;
	SPCTL = 0xd0;	//����1101,0000
	SPSTAT = 0xc0;	//��־λд1����
	//LCD Reset
	R_ESET=1;
	Delay_ms(1);
	R_ESET=0;
	Delay_ms(10);
	R_ESET=1;
	Delay_ms(120);
}

void LCD_Writ_Bus(char a){   //����д�뺯�� 8λ
      C_S = 0;
      SPDAT = a;//��ʼ����
	  while(!(SPSTAT&0x80));  // �ȴ�SPIF = 1,�������  
	  SPSTAT = 0xc0;    
      C_S = 1;
}	

void LCD_WRITE_CMD(char cmd){//8 bit
      C_D = 0;
      LCD_Writ_Bus(cmd);
}

void LCD_WRITE_COM_DATA(char com_data){//8 bit
      C_D = 1;
      LCD_Writ_Bus(com_data);
}

void LCD_WRITE_DATA(int a){//16λ����,�����Σ�ÿ����8λ
      C_D = 1;
      LCD_Writ_Bus(a>>8);
	  LCD_Writ_Bus(a);
}

//���û�㴰�ڣ�x=0~239,y=0~319
void Address_set(unsigned int x1,unsigned int x2,unsigned int y1,unsigned int y2)
{
     	LCD_WRITE_CMD(0x2a); 	//x��   
     	LCD_WRITE_DATA(x1);
     	LCD_WRITE_DATA(x2);
	    LCD_WRITE_CMD(0x2b); 	//y��   
	    LCD_WRITE_DATA(y1);
     	LCD_WRITE_DATA(y2);
     	LCD_WRITE_CMD(0x2c);
}

void LCD_TEST_SingleColor(int Discolor){
     	int i,j;
		Address_set(0,239,0,319);

		for (i=0;i<320;i++){
			for (j=0;j<240;j++){
				LCD_WRITE_DATA(Discolor);
				}
		}
}

//��һ������
void LCD_draw_Line(unsigned int y,unsigned int color)	
{		int i;	
		Address_set(0,239,y,y);
		for (i=0;i<240;i++){
			LCD_WRITE_DATA(color);
			}
}

//Һ�����س�ʼ��
void Initial_ILI9340C(void)
{
        LCD_WRITE_CMD(0xCB);  
        LCD_WRITE_COM_DATA(0x39); 
        LCD_WRITE_COM_DATA(0x2C); 
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x34); 
        LCD_WRITE_COM_DATA(0x02); 

        LCD_WRITE_CMD(0xCF);  
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0XC1); 
        LCD_WRITE_COM_DATA(0X30); 
 
        LCD_WRITE_CMD(0xE8);  
        LCD_WRITE_COM_DATA(0x85); 
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x78); 
 
        LCD_WRITE_CMD(0xEA);  
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x00); 
 
        LCD_WRITE_CMD(0xED);  
        LCD_WRITE_COM_DATA(0x64); 
        LCD_WRITE_COM_DATA(0x03); 
        LCD_WRITE_COM_DATA(0X12); 
        LCD_WRITE_COM_DATA(0X81); 

        LCD_WRITE_CMD(0xF7);  
        LCD_WRITE_COM_DATA(0x20); 
  
        LCD_WRITE_CMD(0xC0);    //Power control 
        LCD_WRITE_COM_DATA(0x23);   //VRH[5:0] 
 
        LCD_WRITE_CMD(0xC1);    //Power control 
        LCD_WRITE_COM_DATA(0x10);   //SAP[2:0];BT[3:0] 
 
        LCD_WRITE_CMD(0xC5);    //VCM control 
        LCD_WRITE_COM_DATA(0x3e); //�Աȶȵ���
        LCD_WRITE_COM_DATA(0x28); 
 
        LCD_WRITE_CMD(0xC7);    //VCM control2 
        LCD_WRITE_COM_DATA(0x86);  //--
 
        LCD_WRITE_CMD(0x36);    // Memory Access Control 
        LCD_WRITE_COM_DATA(0xC8); 

        LCD_WRITE_CMD(0x3A);    
        LCD_WRITE_COM_DATA(0x55); 

        LCD_WRITE_CMD(0xB1);    
        LCD_WRITE_COM_DATA(0x00);  
        LCD_WRITE_COM_DATA(0x18); 
 
        LCD_WRITE_CMD(0xB6);    // Display Function Control 
        LCD_WRITE_COM_DATA(0x08); 
        LCD_WRITE_COM_DATA(0x82);
        LCD_WRITE_COM_DATA(0x27);  
 
        LCD_WRITE_CMD(0xF2);    // 3Gamma Function Disable 
        LCD_WRITE_COM_DATA(0x00); 
 
        LCD_WRITE_CMD(0x26);    //Gamma curve selected 
        LCD_WRITE_COM_DATA(0x01); 
 
        LCD_WRITE_CMD(0xE0);    //Set Gamma 
        LCD_WRITE_COM_DATA(0x0F); 
        LCD_WRITE_COM_DATA(0x31); 
        LCD_WRITE_COM_DATA(0x2B); 
        LCD_WRITE_COM_DATA(0x0C); 
        LCD_WRITE_COM_DATA(0x0E); 
        LCD_WRITE_COM_DATA(0x08); 
        LCD_WRITE_COM_DATA(0x4E); 
        LCD_WRITE_COM_DATA(0xF1); 
        LCD_WRITE_COM_DATA(0x37); 
        LCD_WRITE_COM_DATA(0x07); 
        LCD_WRITE_COM_DATA(0x10); 
        LCD_WRITE_COM_DATA(0x03); 
        LCD_WRITE_COM_DATA(0x0E); 
        LCD_WRITE_COM_DATA(0x09); 
        LCD_WRITE_COM_DATA(0x00); 

        LCD_WRITE_CMD(0XE1);    //Set Gamma 
        LCD_WRITE_COM_DATA(0x00); 
        LCD_WRITE_COM_DATA(0x0E); 
        LCD_WRITE_COM_DATA(0x14); 
        LCD_WRITE_COM_DATA(0x03); 
        LCD_WRITE_COM_DATA(0x11); 
        LCD_WRITE_COM_DATA(0x07); 
        LCD_WRITE_COM_DATA(0x31); 
        LCD_WRITE_COM_DATA(0xC1); 
        LCD_WRITE_COM_DATA(0x48); 
        LCD_WRITE_COM_DATA(0x08); 
        LCD_WRITE_COM_DATA(0x0F); 
        LCD_WRITE_COM_DATA(0x0C); 
        LCD_WRITE_COM_DATA(0x31); 
        LCD_WRITE_COM_DATA(0x36); 
        LCD_WRITE_COM_DATA(0x0F); 
 
        LCD_WRITE_CMD(0x11);    //Exit Sleep 
        Delay_ms(120); 
				
        LCD_WRITE_CMD(0x29);    //Display on 
        LCD_WRITE_CMD(0x2c); 

}

//LCD��������״̬
void LCD_Sleep_ILI9340C(void)
{
  LCD_WRITE_CMD(0x28);//Display off
  Delay_ms(20);
  LCD_WRITE_CMD(0x10);//Enter Sleep mode
}
//LCD�˳�����״̬
void LCD_ExitSleep_ILI9340C(void)
{
  LCD_WRITE_CMD(0x11);//Exit Sleep
  Delay_ms(120);
  LCD_WRITE_CMD(0x29);//Display on
  LCD_WRITE_CMD(0x2c); 
}

void main()
{
		//�򿪱���
		LED1 = 0;
		LED2 = 0;
		LED3 = 0;
		LED4 = 0;
		//Ӳ����ʼ��
		HD_reset();
        Delay_ms(50);//��ʱ50ms
		Initial_ILI9340C();
		Delay_ms(300);
		//���Խ���/�˳�����
		LCD_Sleep_ILI9340C();
		Delay_ms(300);
		LCD_ExitSleep_ILI9340C();
		//��ʾ����
		LCD_TEST_SingleColor(Black); 
		Delay_ms(500);
		while(1){
		//ѭ��6����ɫ����
		LCD_TEST_SingleColor(Red);
		LCD_TEST_SingleColor(Green);
		LCD_TEST_SingleColor(Blue);
		LCD_TEST_SingleColor(Yellow);
		LCD_TEST_SingleColor(White);
		LCD_TEST_SingleColor(Black);
		//y��߶�100����һ����ɫ����
		LCD_draw_Line(100,Red);
		Delay_ms(1000);
		}
}

