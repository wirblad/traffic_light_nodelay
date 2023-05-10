#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include "millis.h"
#include "uart.h"

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))

#define LED_PINLEFT 2
#define LED_PINRIGHT 1
#define LED_SWICTCHCLICKER 3
#define SWITCH_PINLEFT 7
#define SWITCH_PINRIGHT 6

int main(void)
{
    // DATA DIRECTION REGISTER B
    BIT_SET(DDRB, LED_PINLEFT); //Sätt led_pin till output mode
    BIT_SET(DDRB, LED_PINRIGHT); //Sätt led_pin till output mode
    BIT_SET(DDRB,LED_SWICTCHCLICKER);
    BIT_CLEAR(DDRD, SWITCH_PINLEFT); 
    BIT_CLEAR(DDRD, SWITCH_PINRIGHT); 
    BIT_SET(PORTD, SWITCH_PINLEFT); 
    BIT_SET(PORTD, SWITCH_PINRIGHT); 
 //This means INPUT_PULLUP
	// https://forum.arduino.cc/t/using-avr-internal-pull-up-for-push-button-controlling/327729/4
	// https://www.hackster.io/Hack-star-Arduino/push-buttons-and-arduino-a-simple-guide-wokwi-simulator-c2281f    

    millis_init();
    sei();
    init_serial();
    
    volatile millis_t antalMilliSekunderSenSenasteBytet = 0;
    bool isleft = true;
    BIT_SET(PORTB, LED_PINLEFT);
    BIT_CLEAR(PORTB, LED_PINRIGHT); 

    while(1){
        if(BIT_CHECK(PIND,SWITCH_PINLEFT))
            BIT_SET(PORTB,LED_SWICTCHCLICKER);
        else
            BIT_CLEAR(PORTB,LED_SWICTCHCLICKER);

        if( millis_get() - antalMilliSekunderSenSenasteBytet > 3000 )
        {
            if(isleft){
                BIT_CLEAR(PORTB, LED_PINLEFT);
                BIT_SET(PORTB, LED_PINRIGHT); 
            }
            else {
                BIT_SET(PORTB, LED_PINLEFT);
                BIT_CLEAR(PORTB, LED_PINRIGHT); 
            }
            isleft = !isleft;
            antalMilliSekunderSenSenasteBytet = millis(); 
        }   
    }
	return 0;
}