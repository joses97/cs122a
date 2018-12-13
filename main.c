

#define F_CPU 8000000UL	
#include <stdio.h>	
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_ATmega1284.h"
#include "timer.h"




enum States4 {blink4, noblink4} state4;
void BlinkSM4()
{
	switch(state4)
	{
		case blink4:
			state4 = noblink4;
			break;
		case noblink4:
			state4 = blink4;
			break;
		default:
		break;
	}
	switch(state4)
	{
		case blink4:
			PORTC = 0x03;
			break;
		case noblink4:
			PORTC = 0x00;
			break;
		default:
			PORTC = 0x00;
			break;
		
	}
	
	
}


enum States3 {blink3, noblink3} state3;
void BlinkSM3()
{
	switch(state3)
	{
		case blink3:
			state3 = noblink3;
			break;
		case noblink3:
			state3 = blink3;
			 break;
		default:
		break;
	}
	switch(state3)
	{
		case blink3:
			PORTC = 0x02;
			break;
		case noblink3:
			PORTC = 0x00;
			break;
		default:
			PORTC = 0x00;
			break;
				
	}
	
	
}

enum States2 {blink2, noblink2} state2;
void BlinkSM2()
{
	switch(state2)
	{
		case blink2:
			state2 = noblink2; 
			break;
		case noblink2:
			state2 = blink2;		
			break;
		default:
		break;
	}
	switch(state2)
	{
		case blink2:
			PORTC = 0x01;
			break;
		case noblink2:
			PORTC = 0x00;
			break;
		default:
		PORTC = 0x00;
		break;
	}
	
	
}
	
	
	
	
enum States { start, recieve } state;
void tickSM() 
{
	switch (state) //transitions 
	{
		case start: 
			state = recieve; 
			break;
		case recieve:
			state = recieve;
			break;
		default: 
		break; 
	}
	switch (state) //actions 
	{
		case start:
		break;
		case recieve:
		if ( USART_HasReceived(0) ) 
		{
			
			unsigned char tmp;
			tmp = USART_Receive(0);
			if( (tmp == 0x00) | (tmp == 0x05) | (tmp == 0x0A) | (tmp == 0x06) | (tmp == 0x09))
			{
				PORTB = USART_Receive(0);
				USART_Flush(0);
				break;
			}
			if(tmp == 0x10)
			{
				if(OCR1A > 65)
				{
					OCR1A--;
				}
				break;
			}
			else if(tmp == 0x20)
			{
				if(OCR1A < 280)
				{
					OCR1A++;
				}
				break;
			}
					
		}
		break;
	}

}

int main(void)
{
	DDRD |= (1<<PD5);
	TCNT1 = 0;
	ICR1 = 2499;
	
	
	TCCR1A = (1<<WGM11)|(1<<COM1A1);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	
	
	
	initUSART(0);
	TimerSet(5);
	TimerOn();
	unsigned char temp = 0;
	while (1)
	{	
		if( (PINA&0x01) == 0x00)
		{
			BlinkSM2();
		}
		if( (PINA&0x02) == 0x00)
		{
			BlinkSM3();
		}
		if( (PINA&0x03) == 0x00)
		{
			BlinkSM4();
		}
		
		tickSM();
		while(!TimerFlag);
		TimerFlag = 0;
		PORTC = 0x00;
	}
    
}
