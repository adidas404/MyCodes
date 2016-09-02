#include "defines.h"
#include "stm32f4xx.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_sdram.h"
#include "tm_stm32f4_spi.h"
#include "tm_stm32f4_pwm.h"
#include "tm_stm32f4_gpio.h"

#include "tm_stm32f4_fatfs.h"
#include "tm_stm32f4_usb_msc_host.h"

#include <stdio.h>


//prototype function ha
void spi24(uint8_t* sendout);
void tabdil(char* x);


FATFS USB_Fs;
FIL USB_Fil;

int main(void) {
	char buffer;
	uint32_t address=0, RamAdd=0;
	uint8_t send[3]={0x00,0x00,0x00};
	int a,b;
	a=0; b=0;
	
	
	
	// Initialize
	SystemInit();
	TM_DELAY_Init();
	TM_SDRAM_Init();
	TM_DISCO_LedInit();
	TM_DISCO_ButtonInit();
	TM_USB_MSCHOST_Init();
	
	
	//FLASH MEMORY READ & STORE

	while (1) {
		TM_USB_MSCHOST_Process();
		if (TM_USB_MSCHOST_Device() == TM_USB_MSCHOST_Result_Connected) {
				if (f_mount(&USB_Fs, "1:", 1) == FR_OK) {
					if (f_open(&USB_Fil, "1:test.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS) == FR_OK) {
						
						//khandan az flash
						for(int i=0;1==1;i=i+6)
						{
							//2 character avval
							f_lseek(&USB_Fil,i);
							f_gets(&buffer,1,&USB_Fil);
							if(buffer=='X')
								break;
							tabdil(&buffer);
							a=buffer*16;
							
							f_lseek(&USB_Fil,i+1);
							f_gets(&buffer,1,&USB_Fil);
							tabdil(&buffer);
							b=buffer;
							
							TM_SDRAM_Write8(RamAdd++, a+b);
							
							
							
							//2 character dovvom
							f_lseek(&USB_Fil,i+2);
							f_gets(&buffer,1,&USB_Fil);
							tabdil(&buffer);
							a=buffer*16;
							
							f_lseek(&USB_Fil,i+3);
							f_gets(&buffer,1,&USB_Fil);
							tabdil(&buffer);
							b=buffer;
							
							TM_SDRAM_Write8(RamAdd++, a+b);
							//RamAdd=tedad khane haye 8bit Ram ke por shode+1
						}
							
						f_close(&USB_Fil);
					
					}						
					}
			
				f_mount(0, "1:", 1);
				TM_DISCO_LedOff(LED_RED);
				break;
					
		} else {
			// Not inserted, turn on RED led 
			TM_DISCO_LedOn(LED_RED);
			TM_DISCO_LedOff(LED_GREEN);
			
		}
		
	} //FLASH MEMORY READ & STORE
	
	
		TM_DISCO_LedOff(LED_GREEN);
		
	//aghaze barname qabli
	
	GPIO_InitTypeDef GPIO_InitDef;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//GPIO  Pin (PA11)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_11 ;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitDef);
	//Bin~2s Pin (PA9)
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOA, &GPIO_InitDef);
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	//busy(PG6)  & pec(PG3) Pin
	GPIO_InitDef.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_3 ;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitDef.GPIO_OType = GPIO_OType_OD;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOG, &GPIO_InitDef);
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	//Reset(PC7) & Clr(PC9) Pin
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_9;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOC, &GPIO_InitDef);
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//Synch(PB0) Pin
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
  GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOB, &GPIO_InitDef);
	
	
	
	//SPI 5, PP 1 initialized (SCLK: PF7, SDO: PF8, SDI: PF9)
	TM_SPI_Init(SPI5, TM_SPI_PinsPack_1);
	
	
	
	
	//reset low
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	Delay(5);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	//zamani baraye initialize shodane register haye C o M o ...
	Delay(500);
	
	//Bin~2s low
	GPIO_ResetBits(GPIOA, GPIO_Pin_9);
	//CLR high
	GPIO_SetBits(GPIOC, GPIO_Pin_9);
	
	
	
	
	
	
	
	//control register
	send[0]=0x01; send[1]=0x00; send[2]=0x02;
	spi24(send);
	Delay(10);
	
	//Monitor register
	send[0]=0x0C; send[1]=0x00; send[2]=0x00;
	spi24(send);
	Delay(10);
	
	//A~B select register
	send[0]=0x0B; send[1]=0x00; send[2]=0x00;
	spi24(send);
	Delay(10);
	
	//GPIO register
	send[0]=0x0D; send[1]=0x00; send[2]=0x03;
	spi24(send);
	Delay(10);
	
	
	
	
	
	//pwm initialized (PA3)
	TM_PWM_TIM_t TIM5_Data;
	TM_PWM_InitTimer(TIM5, &TIM5_Data,10000);
	TM_PWM_InitChannel(&TIM5_Data, TM_PWM_Channel_4, TM_PWM_PinsPack_1);
	TM_PWM_SetChannelPercent(&TIM5_Data, TM_PWM_Channel_4, 98);
	
	

		send[0]=0xC8;
		
		while(!TM_DISCO_ButtonPressed());
	  
		TM_DISCO_LedOn(LED_GREEN);
		
		
		
		
		
		
	//halqeye while ke bayad ta zamani ke data hast edame yabad
	while(address<RamAdd) {
		
		
		
		
		//dac 0
		send[0]=0xC8;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		
		
		
		//dac 1
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 2
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 3
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		
		
		//dac 4
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 5
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 6
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 7
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 8
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 9
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 10
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 11
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 12
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		
		//dac 13
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 14
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		//dac 15
		send[0]++;
		send[1]=TM_SDRAM_Read8(address++);
		send[2]=TM_SDRAM_Read8(address++);
		spi24(send);
		
		
		
		//sabr mikonim ta ldac low shavad
		while(GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_3));
		while(!(GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_3)));
		
	
		Delay(1);
		
		
	}
	//payane while 

}



//function ha

void spi24(uint8_t* sendout)
{
	int d=100;
	
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
	
	while(d!=95)
		--d;
	
	TM_SPI_WriteMulti(SPI5, sendout, 3);
	
	while(d!=89)
		--d;
	
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
	
}



void tabdil(char* x){
	
	if( (*x>47) && (*x<58))
		*x=*x-48;
	
	else if( (*x>64) && (*x<71))
		*x=*x-55;
	
	else
		*x=0;
}



