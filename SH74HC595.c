/*      
        SN74HC595 SHIFT REGISTER SIPO
////    PA0 SRCLK (D FF CLOCK USED TO SHIFT BIT)
////    PA1 RCLK  (SR FF CLOCK USED TO STORE VALUES)
///     PA2 SER   (INPUT PIN TO GET THE SERIAL DATA)
*/


#include <avr/io.h>
#include <util/delay.h>

//////  MACROS  //////
#define SRPORT PORTA
#define SRDDR   DDRA
#define clockPin PA0
#define latchPin PA1
#define dataPin  PA2
#define clrpin PB0


void init()
{
     SRDDR |=(1<< clockPin | 1<< latchPin| 1<< dataPin); ///configuring as output pin    
     DDRB |=(1<clrpin);
     PORTB &=~(1<<clrpin);
     PORTB |=(1<<clrpin);
}

void highDataPin()
{
    SRPORT|=(1<<dataPin);   ///// make high when the input is one
}

void lowDataPin()
{
    SRPORT&=~(1<<dataPin);  /// make low when the input is zero
}

void clr()
{
    SRPORT |=(1<<clockPin);
    PORTB |=(1<<clrpin);
    PORTB&=~(1<<clrpin);
    SRPORT &=~(1<<clockPin);

}


void writeSr(uint8_t data)
{
        uint8_t pattern=data;
        SRPORT&=~(1<<latchPin);     ////MAKING OUTPUT CLOCK ZERO(STORAGE CLOCK) DISABLE THE OUTPUT

    for(int j=0;j<9;j++)
        {
        SRPORT|=(1<<clockPin);      ////SET THE SHIFT REGISTER CLOCK(SRCLOCK)
        if(pattern & 0b10000000)
        {
            highDataPin();
        }
        else
        {
            lowDataPin();
        }
        pattern=pattern<<1;
        SRPORT &=~(1<<clockPin);  //// LOW THE SHIFT REGISTER CLOCK 
        }
        SRPORT|=(1<<latchPin);   ///SETTING THE LATCH CLOCK TO ENABLE THE OUTPUT
}



int main()
{
    init();
    uint8_t data[6]= {0b00001111,0b11111111,0b01010101,0b10101010,0b11001100,0b10011001};
    while(1)
        {   
            for(int i=0;i<6;i++)
            { 
            writeSr(data[i]);
             _delay_ms(600);
            }   
        }
    return 0;
}


/////VOID HIGH() AND VOID LOW() THIS FUNCTION IS AS AN SERIAL INPUT PIN 
