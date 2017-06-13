/** Samet KOCA 140202037 **/

#include "inc/lm4f120h5qr.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
//ADC icin
#include "inc/hw_memmap.h"
#include "inc/hw_adc.h"
#include "driverlib/adc.h"

void Display(unsigned char binary);
void Clear(void);

volatile unsigned long delay;
volatile int Random;

void PortInit()
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; //PortD icin saati aktif eder
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; //PortE icin saati aktif eder
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; //PortB icin saati aktif eder
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTD_DIR_R = 0xFF; //PortD icin 7,6,5,4,3,2,1,0 pinlerini cikis yapar
	GPIO_PORTD_DEN_R = 0xFF; //PortD icin 7,6,5,4,3,2,1,0 pinlerini aktifleştirir
	GPIO_PORTD_AFSEL_R &= ~0xFF;

	GPIO_PORTB_DIR_R |= 0x0F; //PortB icin 3,2,1,0 pinlerini cikis yapar
    GPIO_PORTB_AFSEL_R &= ~0x0F; //PortB 3,2,1,0 pinlerinin alternatif fonksiyonunu 0 yapar
    GPIO_PORTB_DEN_R |= 0x0F; //PortB icin 3,2,1,0 pinlerini aktifleştirir

    GPIO_PORTE_DIR_R = 0xFF; //PortE icin 7,6,5,4,3,2,1,0 pinlerini cikis yapar
	GPIO_PORTE_DEN_R = 0xFF; //PortE icin 7,6,5,4,3,2,1,0 pinlerini aktifleştirir
	Display(0x28); //4bit modunu başlatir
	Display(0x0c); //Imleci gorunmez yapar
	Clear();
	Display(0x6); //Imleci otomatik olarak arttirir
}

void Write(char *yazi)
{
	while(*yazi){

		unsigned char Character = *yazi++;

		GPIO_PORTD_DATA_R |=  0b00000001; //RS
	    SysCtlDelay(400);
        GPIO_PORTE_DATA_R = (Character & 0xf0) >> 4; //Gelen 8bitlik sayi 4 birim saga kayar
	    SysCtlDelay(50000);
	    GPIO_PORTD_DATA_R |=  0b00000010; //EN
	    GPIO_PORTD_DATA_R &= ~0b00000010; //EN
		GPIO_PORTE_DATA_R = Character; //Gelen 8bitkik sayinin sol 4 biti alinir
	    GPIO_PORTD_DATA_R |=  0b00000010; //EN
		GPIO_PORTD_DATA_R &= ~0b00000010; //EN
	}
}

void Display(unsigned char binary)
{
	GPIO_PORTD_DATA_R &= ~0b00000001; //RS
    SysCtlDelay(400);
    GPIO_PORTE_DATA_R = binary >> 4;
    GPIO_PORTD_DATA_R |=  0b00000010; //EN
    GPIO_PORTD_DATA_R &= ~0b00000010; //EN
    GPIO_PORTE_DATA_R = binary;
    GPIO_PORTD_DATA_R |=  0b00000010; //EN
    GPIO_PORTD_DATA_R &= ~0b00000010; //EN
}

void Clear(void) //Tum ekranı siler
{
    for(int i=0;i<16;i++){
    	Display(0x80+15-i);//Satiri hep bir sola kaydir
    	Write(" ");
    }
    for(int i=0;i<16;i++){
    	Display(0xC0+15-i); //Satiri hep bir sola kaydir
    	Write(" ");
    }
    SysCtlDelay(15000);
}

int main()
{
	PortInit(); //Portlari acar
	Clear();
	int i;
	char *School = "KOCAELI UNI";
	char *Name = "SAMET KOCA";

	int ADCValue[8];

	SysCtlClockSet( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_25MHZ); //Saati ayarlar

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	ADCSequenceStepConfigure(ADC0_BASE, 1, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 1);

    while(1){

    	ADCIntClear(ADC0_BASE, 1);
    	ADCProcessorTrigger(ADC0_BASE, 1);

    	while (!ADCIntStatus(ADC0_BASE, 1, false))
    	ADCSequenceDataGet(ADC0_BASE, 1, ADCValue);

        Random=ADCValue[0] % 4;

        if(Random == 0){//Kırmızı Led Önce okul
	    	GPIO_PORTB_DATA_R |=  0b00000001; //Kirmizi ledi yak digerini sondur
         	GPIO_PORTB_DATA_R &= ~0b00000010;
	    	GPIO_PORTB_DATA_R &= ~0b00000100;
	    	GPIO_PORTB_DATA_R &= ~0b00001000;
	    	Display(0x80);
		  	Write(School);
	     	SysCtlDelay(20000000);
	     	Display(0xc0);
			Write(Name);
	    }

	    if(Random == 1){ //Beyaz led Önce isim
		    GPIO_PORTB_DATA_R |=  0b00000010; //Beyaz ledi yak digerini sondur
	    	GPIO_PORTB_DATA_R &= ~0b00000001;
	    	GPIO_PORTB_DATA_R &= ~0b00000100;
	    	GPIO_PORTB_DATA_R &= ~0b00001000;

	    	Display(0x80);
	    	Display(0xc0);
	    	Write(Name);
	    	SysCtlDelay(30000000);
	    	Display(0x80);
	    	Write(School);
	   }

	   if(Random == 2){ //Yesil led Soldan saga
	     	GPIO_PORTB_DATA_R |=  0b00000100; //Yesil ledi yak digerini sondur
	    	GPIO_PORTB_DATA_R &= ~0b00000001;
	    	GPIO_PORTB_DATA_R &= ~0b00000010;
	    	GPIO_PORTB_DATA_R &= ~0b00001000;

	    	for(i=0;i<11;i++){
	    		Display(0x80);
			    Write(10+School-i);
			    SysCtlDelay(1000000);
		    }
	    	SysCtlDelay(20000000);
		    for(i=0;i<10;i++){
		    	Display(0xc0);
		    	Write(9+Name-i);
		    	SysCtlDelay(1000000);
		    }
	    }

	    if(Random == 3){ //Mavi led Sagdan Sola
		   	GPIO_PORTB_DATA_R |=  0b00001000; //Mavi ledi yak digerini sondur
	    	GPIO_PORTB_DATA_R &= ~0b00000001;
		   	GPIO_PORTB_DATA_R &= ~0b00000010;
		   	GPIO_PORTB_DATA_R &= ~0b00000100;

		   	for(i=0;i<16;i++){

	    		for(int j=0;j<16;j++){
	    	    	Display(0x80+15-j);
	             	Write(" ");
	    		}

	    		Display(0x80+15-i);
				Write(School);
	    		SysCtlDelay(1000000);
		   	}

		   	SysCtlDelay(20000000);

		 	for(i=0;i<16;i++){

		   	    for(int j=0;j<16;j++){
		   	     	Display(0xC0+15-j);
		   	     	Write(" ");
		   	     }

		 		Display(0xC0+15-i);
	     	   	Write(Name);
			   	SysCtlDelay(1000000);
			}
	     }

	     SysCtlDelay(30000000);
	     Clear();
	}

}
