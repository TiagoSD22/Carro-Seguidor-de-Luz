#include<p18f4520.h>
#include <delays.h>	

#pragma config OSC      = INTIO67
#pragma config WDT      = OFF, MCLRE = OFF         
#pragma config DEBUG    = ON, LVP = OFF, PWRT = ON 
#pragma config PBADEN   = OFF 

#define saida1 LATDbits.LATD0
#define saida2 LATDbits.LATD1
#define Tensao_REF 3.0

void ADCInit()
{
   ADCON2=0b10001010;
}

unsigned int ADCRead(unsigned char ch)
{
   if(ch>13) return 0;  

   ADCON0=0x00;

   ADCON0=(ch<<2);  

   ADCON0bits.ADON=1; 

   ADCON0bits.GO=1;

   while(ADCON0bits.GO); 

   ADCON0bits.ADON=0;  

   return ADRES;
}

void Configurar_Pic(void){
	OSCCON = 0x70;
	TRISA = 0b00000011;
	TRISB = 0b11111111;
	TRISC = 0b00000000;
	TRISD = 0b00000000;
	TRISE = 0b00000000;
	
	LATA = 0b00000000;
	LATB = 0b00000000;
	LATC = 0b00000000;
	LATD = 0b00000000;
	LATE = 0b00000000;
}

void main(void){
	float leitura_ldr1,leitura_ldr2,tensao1,tensao2;
	Configurar_Pic();
	ADCInit();
	while(1){
		leitura_ldr1 = (float)ADCRead(0);
		Delay10TCYx(1);
		leitura_ldr2 = (float)ADCRead(1);
		Delay10TCYx(1);
		tensao1 = (float)(leitura_ldr1 * 5.0 ) / 1024.0;
		tensao2 = (float)(leitura_ldr2 * 5.0 ) / 1024.0;
		if(tensao1 > Tensao_REF){
			if(tensao2 > Tensao_REF){
				saida1 = 1;
				saida2 = 1;
			}
			else{
				saida1 = 1;
				saida2 = 0;
			}
		}
		else{
			if(tensao2 > Tensao_REF){
				saida1 = 0;
				saida2 = 1;
			}
			else{
				saida1 = 0;
				saida2 = 0;
			}
		}
	}
}