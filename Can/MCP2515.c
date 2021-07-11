/**********************************************************************************
 * �ļ���  ��MCP2515.c
 * ����    ��MCP2515����������         
 * ʵ��ƽ̨��NiRen_STC/IAP15���İ�(���û�STC15��Ƭ��������) + NiRen_MCP2515 CANģ��    
**********************************************************************************/

#include <reg51.h>
#include "MCP2515.H"

//MCP2515���Ŷ���
sbit MCP2515_INT  = P1^6;//MCP2515�ж�����		������û���õ���

sbit MCP2515_SCK  = P1^5;//SPIʱ������ 
sbit MCP2515_MOSI = P1^4;//SPI��������ӻ��������� 
sbit MCP2515_MISO = P1^3;//SPI��������ӻ�������� 
sbit MCP2515_CS   = P1^2;//SPIƬѡ����

unsigned char code TXB0_Address[]={TXB0CTRL,TXB0SIDH,TXB0SIDL,TXB0EID8,TXB0EID0,TXB0DLC,TXB0D0,TXB0D1,TXB0D2,TXB0D3,TXB0D4,TXB0D5,TXB0D6,TXB0D7};
unsigned char code TXB1_Address[]={TXB1CTRL,TXB1SIDH,TXB1SIDL,TXB1EID8,TXB1EID0,TXB1DLC,TXB1D0,TXB1D1,TXB1D2,TXB1D3,TXB1D4,TXB1D5,TXB1D6,TXB1D7};
unsigned char code TXB2_Address[]={TXB2CTRL,TXB2SIDH,TXB2SIDL,TXB2EID8,TXB2EID0,TXB2DLC,TXB2D0,TXB2D1,TXB2D2,TXB2D3,TXB2D4,TXB2D5,TXB2D6,TXB2D7};
 
unsigned char code RXF0_Address[]={RXF0SIDH,RXF0SIDL,RXF0EID8,RXF0EID0};
unsigned char code RXF1_Address[]={RXF1SIDH,RXF1SIDL,RXF1EID8,RXF1EID0};
unsigned char code RXF2_Address[]={RXF2SIDH,RXF2SIDL,RXF2EID8,RXF2EID0};
unsigned char code RXF3_Address[]={RXF3SIDH,RXF3SIDL,RXF3EID8,RXF3EID0};
unsigned char code RXF4_Address[]={RXF4SIDH,RXF4SIDL,RXF4EID8,RXF4EID0};
unsigned char code RXF5_Address[]={RXF5SIDH,RXF5SIDL,RXF5EID8,RXF5EID0};

unsigned char code RXM0_Address[]={RXM0SIDH,RXM0SIDL,RXM0EID8,RXM0EID0};
unsigned char code RXM1_Address[]={RXM1SIDH,RXM1SIDL,RXM1EID8,RXM1EID0};

/*******************************************************************************
* ������  : Delay_Nms
* ����    : ͨ��������ʱԼnms(��׼ȷ)
* ����    : x
* ˵��    : �˷�ʽ��ʱʱ���ǲ�׼ȷ��,׼ȷ��ʱ�����ö�ʱ��
*******************************************************************************/
void Delay_Nms(unsigned int x)
{
	unsigned int y;

	for (;x>0;x--)
		for (y=0;y<100;y++);
}


/*******************************************************************************
* ������  : SPI_ReadByte
* ����    : ͨ��SPI��ȡһ���ֽ�����
* ����    : ��
* ���    : ��
* ����ֵ  : rByte(��ȡ����һ���ֽ�����)
* ˵��    : ��
*******************************************************************************/
unsigned char SPI_ReadByte(void)
{
	unsigned char i,rByte=0;
	
	MCP2515_SCK=0;
	for(i=0;i<8;i++)
	{
		MCP2515_SCK=1;
		rByte<<=1;
		rByte|=MCP2515_MISO;
		MCP2515_SCK=0;	
	}
	return rByte;
}

/*******************************************************************************
* ������  : SPI_SendByte
* ����    : SPI����һ���ֽ�����
* ����    : dt:�����͵�����
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void SPI_SendByte(unsigned char dt)
{
	unsigned char i;
		
	for(i=0;i<8;i++)
	{	
		MCP2515_SCK=0;
		if((dt<<i)&0x80)
			MCP2515_MOSI=1;
		else
			MCP2515_MOSI=0;					
		MCP2515_SCK=1;
	}
	MCP2515_SCK=0;
}

/*******************************************************************************
* ������  : MCP2515_WriteByte
* ����    : ͨ��SPI��MCP2515ָ����ַ�Ĵ���д1���ֽ�����
* ����    : addr:MCP2515�Ĵ�����ַ,dat:��д�������
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void MCP2515_WriteByte(unsigned char addr,unsigned char dat)
{
	MCP2515_CS=0;				//��MCP2515��CSΪ�͵�ƽ
	SPI_SendByte(CAN_WRITE);	//����д����
	SPI_SendByte(addr);			//���͵�ַ
	SPI_SendByte(dat);			//д������
	MCP2515_CS=1;				//��MCP2515��CSΪ�ߵ�ƽ 
}

/*******************************************************************************
* ������  : MCP2515_ReadByte
* ����    : ͨ��SPI��MCP2515ָ����ַ������1���ֽ�����
* ����    : addr:MCP2515�Ĵ�����ַ
* ���    : ��
* ����ֵ  : rByte:��ȡ���Ĵ�����1���ֽ�����
* ˵��    : ��
*******************************************************************************/
unsigned char MCP2515_ReadByte(unsigned char addr)
{
	unsigned char rByte;
	
	MCP2515_CS=0;				//��MCP2515��CSΪ�͵�ƽ
	SPI_SendByte(CAN_READ);		//���Ͷ�����
	SPI_SendByte(addr);			//���͵�ַ
	rByte=SPI_ReadByte();		//��ȡ����
	MCP2515_CS=1;				//��MCP2515��CSΪ�ߵ�ƽ
	return rByte;				//���ض�����һ���ֽ�����
}

/*******************************************************************************
* ������  : MCP2515_Reset
* ����    : ���͸�λָ��������λMCP2515
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ���ڲ��Ĵ�����λΪȱʡ״̬,���������趨Ϊ����ģʽ
*******************************************************************************/
void MCP2515_Reset(void)
{
	MCP2515_CS=0;				//��MCP2515��CSΪ�͵�ƽ
	SPI_SendByte(CAN_RESET);	//���ͼĴ�����λ����
	MCP2515_CS=1;				//��MCP2515��CSΪ�ߵ�ƽ
}

/*******************************************************************************
* ������  : CAN_Set_RX
* ����    : ������ģʽ������������RXM�͹�����RXF
* ����    : *CAN_TX_Buf(���������ݻ�����ָ��),len(���������ݳ���)
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void CAN_Set_RX(unsigned char RXF_Address,unsigned long int ID,unsigned char EXIDE)
{
	//EXIDE=1��ID>0x7FF������չ֡
	if (ID<=0x7FF)
	{
		if (EXIDE)
		{
			MCP2515_WriteByte(RXF_Address+0,0x0);								  //SIDH
			MCP2515_WriteByte(RXF_Address+1,0x8);								  //SIDL
			MCP2515_WriteByte(RXF_Address+2,ID>>8&0xFF);						  //EID8
			MCP2515_WriteByte(RXF_Address+3,ID&0xFF);							  //EID0
		}																	  
		else
		{	
			MCP2515_WriteByte(RXF_Address+0,ID>>3);							  //SIDH
			MCP2515_WriteByte(RXF_Address+1,(ID&0x07)<<5);					  //SIDL
			MCP2515_WriteByte(RXF_Address+2,0);								  //EID8
			MCP2515_WriteByte(RXF_Address+3,0);								  //EID0
		}
	}
	else
	{	
		MCP2515_WriteByte(RXF_Address+0,ID>>21);								  //SIDH
		MCP2515_WriteByte(RXF_Address+1,(ID>>18&0x07)<<5|(ID>>16&0x03)|0x08);	  //SIDL
		MCP2515_WriteByte(RXF_Address+2,ID>>8&0xFF);							  //EID8
		MCP2515_WriteByte(RXF_Address+3,ID&0xFF);								  //EID0
	}
}


/*******************************************************************************
* ������  : MCP2515_Init
* ����    : MCP2515��ʼ������
* ����    : ��
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��ʼ��������������λ���������������á���ʶ��������õȡ�
*******************************************************************************/
void MCP2515_Init(unsigned char *CAN_Bitrate)
{
	unsigned char temp=0;

	MCP2515_Reset();	//���͸�λָ��������λMCP2515
	Delay_Nms(1);		//ͨ��������ʱԼnms(��׼ȷ)

	//���ò�����
	//set CNF1,SJW=00,����Ϊ1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/8M=12.5us
	MCP2515_WriteByte(CNF1,CAN_Bitrate[4]|CAN_Bitrate[0]);
	//set CNF2,SAM=0,�ڲ���������߽���һ�β�����PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ
	MCP2515_WriteByte(CNF2,BTLMODE_CNF3|CAN_Bitrate[2]|CAN_Bitrate[1]);
	//set CNF3,PHSEG2=(2+1)TQ=3TQ,ͬʱ��CANCTRL.CLKEN=1ʱ�趨CLKOUT����Ϊʱ�����ʹ��λ
	MCP2515_WriteByte(CNF3,SOF_ENABLED|CAN_Bitrate[3]);

	MCP2515_WriteByte(RXB0CTRL,0x06);//���RXB0��,RXB0 ���յ��ı��Ľ���������RXB1

	//unsigned char RXF_Address,unsigned long int ID,unsigned char EXIDE)	
	//RXB0 ���ջ������䱸�������˲��Ĵ���RXF0 ��RXF1���Լ��������μĴ���RXM0��
	CAN_Set_RX(RXF0SIDH,0x100,1);
	CAN_Set_RX(RXF1SIDH,0x7FE,0);

	CAN_Set_RX(RXM0SIDH,0x7FF,0);

	//RXB1 �䱸�������˲��Ĵ���RXF2��RXF3��RXF4��RXF5���˲����μĴ���RXM1��
	CAN_Set_RX(RXF2SIDH,0x800,1);
	CAN_Set_RX(RXF3SIDH,0x1FFFFFFF,1);
	CAN_Set_RX(RXF4SIDH,0x7FF,0);
	CAN_Set_RX(RXF5SIDH,0x0,0);
	
	CAN_Set_RX(RXM1SIDH,0x1FFFFFFE,0);

//	MCP2515_WriteByte(TXB0SIDH,0xAB);//���ͻ�����0��׼��ʶ����λ
//	MCP2515_WriteByte(TXB0SIDL,0xE0);//|0x08|0x2);//���ͻ�����0��׼��ʶ����λ
//	MCP2515_WriteByte(TXB0EID8,0x00);//���ͻ�����0��׼��ʶ����λ
//	MCP2515_WriteByte(TXB0EID0,0x00);//���ͻ�����0��׼��ʶ����λ
//
//
//	MCP2515_WriteByte(RXB0SIDH,0x00);//��ս��ջ�����0�ı�׼��ʶ����λ
//	MCP2515_WriteByte(RXB0SIDL,0x00);//��ս��ջ�����0�ı�׼��ʶ����λ
//
//	MCP2515_WriteByte(RXB0CTRL,0x62);//�������ձ�׼��ʶ������Ч��Ϣ
//	MCP2515_WriteByte(RXB0DLC,DLC_8);//���ý������ݵĳ���Ϊ8���ֽ�
//	
//	MCP2515_WriteByte(RXF0SIDH,0xFF);//���������˲��Ĵ���n��׼��ʶ����λ
//
//	MCP2515_WriteByte(RXM0SIDH,0xFF);//�����������μĴ���n��׼��ʶ����λ
//	MCP2515_WriteByte(RXM1EID0,0xE0);//�����������μĴ���n��׼��ʶ����λ
//		
	MCP2515_WriteByte(CANINTF,0x00);//���CAN�жϱ�־�Ĵ���������λ(������MCU���)
	MCP2515_WriteByte(CANINTE,0x03);//����CAN�ж�ʹ�ܼĴ����Ľ��ջ�����0���ж�ʹ��,����λ��ֹ�ж�
	
	MCP2515_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//|OSM_ENABLED��MCP2515����Ϊ����ģʽ,�˳�����ģʽ
	
	temp=MCP2515_ReadByte(CANSTAT);//��ȡCAN״̬�Ĵ�����ֵ
	if(OPMODE_NORMAL!=(temp&&0xE0))//�ж�MCP2515�Ƿ��Ѿ���������ģʽ
	{
		MCP2515_WriteByte(CANCTRL,REQOP_NORMAL|CLKOUT_ENABLED);//|OSM_ENABLED�ٴν�MCP2515����Ϊ����ģʽ,�˳�����ģʽ
	}
}

/*******************************************************************************
* ������  : CAN_Send_Buffer
* ����    : CAN����ָ�����ȵ�����
* ����    : *CAN_TX_Buf(���������ݻ�����ָ��),len(���������ݳ���)
* ���    : ��
* ����ֵ  : ��
* ˵��    : ��
*******************************************************************************/
void CAN_Send_buffer(unsigned long int ID,unsigned char EXIDE,unsigned char DLC,unsigned char *Send_data)
{
	unsigned char *TXB_send;
   	unsigned char i;
	//�Ĵ���״̬��ȡTXB0�Ƿ�æµ��=1Ϊæµ��=0Ϊ����
	if ((MCP2515_ReadByte(TXB0CTRL)&0x04)==0) 
		TXB_send=&TXB0_Address;
	else if ((MCP2515_ReadByte(TXB1CTRL)&0x04)==0)
		TXB_send=&TXB1_Address;
	else if ((MCP2515_ReadByte(TXB2CTRL)&0x04)==0)
		TXB_send=&TXB2_Address;
	else		  //�Ĵ���ȫ��æµ
		TXB_send=&TXB0_Address;

	//����DLC
	MCP2515_WriteByte(TXB_send[5],DLC);

	//EXIDE=1��ID>0x7FF������չ֡
	if (ID<=0x7FF)
	{
		if (EXIDE)
		{
			MCP2515_WriteByte(TXB_send[1],0x0);								  //SIDH
			MCP2515_WriteByte(TXB_send[2],0x8);								  //SIDL
			MCP2515_WriteByte(TXB_send[3],ID>>8&0xFF);						  //EID8
			MCP2515_WriteByte(TXB_send[4],ID&0xFF);							  //EID0
		}																	  
		else
		{	
			MCP2515_WriteByte(TXB_send[1],ID>>3);							  //SIDH
			MCP2515_WriteByte(TXB_send[2],(ID&0x07)<<5);					  //SIDL
			MCP2515_WriteByte(TXB_send[3],0);								  //EID8
			MCP2515_WriteByte(TXB_send[4],0);								  //EID0
		}
	}
	else
	{	
		MCP2515_WriteByte(TXB_send[1],ID>>21);								  //SIDH
		MCP2515_WriteByte(TXB_send[2],(ID>>18&0x07)<<5|(ID>>16&0x03)|0x08);	  //SIDL
		MCP2515_WriteByte(TXB_send[3],ID>>8&0xFF);							  //EID8
		MCP2515_WriteByte(TXB_send[4],ID&0xFF);								  //EID0
	}
	
	for(i=0;(i<=DLC&&i<=8);i++)
	{
		MCP2515_WriteByte(TXB_send[6]+i,Send_data[i]);						  //D0_8�������͵�����д�뷢�ͻ���Ĵ���
	}

	MCP2515_CS=0;
	MCP2515_WriteByte(TXB_send[0],0x08);									  //CTRL ��ʼ����
	MCP2515_CS=1;
	
	//������ʽ���ã����η��ͣ�Զ��֡����չ֡������״̬����Ӧ��
}

/*******************************************************************************
* ������  : CAN_Receive_Buffer
* ����    : CAN����һ֡����
* ����    : RX_Address(��Ҫ��ȡ��RXB0CTRL��RXB1CTRL��ַ),*CAN_TX_Buf(���������ݻ�����ָ��)
* ���    : ��
* ����ֵ  : len(���յ����ݵĳ���,0~8�ֽ�)
* ˵��    : ��
*******************************************************************************/
void CAN_Receive_Buffer(unsigned char RXB_CTRL_Address,unsigned char *CAN_RX_Buf)
{	
	unsigned char j;
	for(j=0;j<14;j++)
	{
		CAN_RX_Buf[j]=MCP2515_ReadByte(RXB_CTRL_Address+j);//��CAN���յ������ݷ���ָ��������
		//CAN_RX_Buf[j]=RXB_CTRL_Address+j;
	}
	if (RXB_CTRL_Address==RXB0CTRL)
		{
		MCP2515_WriteByte(CANINTF,0);//����жϱ�־λ(�жϱ�־�Ĵ���������MCU����)
		}
	else if (RXB_CTRL_Address==RXB1CTRL)
		{
		MCP2515_WriteByte(CANINTF,0);//����жϱ�־λ(�жϱ�־�Ĵ���������MCU����)	
		}
}

