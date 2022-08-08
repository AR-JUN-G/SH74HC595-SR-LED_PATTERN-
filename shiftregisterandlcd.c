/*      
        SN74HC595 SHIFT REGISTER SIPO
////    PA0 SRCLK (D FF CLOCK USED TO SHIFT BIT)
////    PA1 RCLK  (SR FF CLOCK USED TO STORE VALUES)
///     PA2 SER   (INPUT PIN TO GET THE SERIAL DATA)
*/


#include <avr/io.h>
#include <util/delay.h>

///////////////////////////////////////// MACROS  /////////////////////////////////////////////////

//////  SHIFT REGISTER   //////////
#define clockPin PA0
#define latchPin PA1
#define dataPin  PA2
#define clrpin PA3

///LCD MACRO //////////////////
#define RS PB0
#define EN PB2

/////   BMP280  ////////////
#define SCL PA4
#define SDI PA6
#define CSB PA7
#define SDO PA5

//////////////////////////////////////////  MACROS  /////////////////////////////////////////////////

void enablePulse(){
	PORTB &=~(1<<EN);
	_delay_ms(1);
	PORTB |=(1<<EN);
}



void highDataPin()
{
    PORTA|=(1<<dataPin);   ///// make high when the input is one
}

void lowDataPin()
{
    PORTA&=~(1<<dataPin);  /// make low when the input is zero
}

void clr()
{
    PORTA |=(1<<clockPin);
    PORTB |=(1<<clrpin);
    PORTB&=~(1<<clrpin);
    PORTA &=~(1<<clockPin);

}



//////      shift register    ///////////////
void writeSr(uint8_t data)
{
        uint8_t pattern=data;
        PORTA&=~(1<<latchPin);     ////MAKING OUTPUT CLOCK ZERO(STORAGE CLOCK) DISABLE THE OUTPUT

    for(int j=0;j<9;j++)
        {
        PORTA|=(1<<clockPin);      ////SET THE SHIFT REGISTER CLOCK(SRCLOCK)
        if(pattern & 0b10000000)
        {
            highDataPin();
        }
        else
        {
            lowDataPin();
        }
        pattern=pattern<<1;
        PORTA&=~(1<<clockPin);  //// LOW THE SHIFT REGISTER CLOCK 
        }
        PORTA|=(1<<latchPin);   ///SETTING THE LATCH CLOCK TO ENABLE THE OUTPUT
        _delay_us(100);
}


void writeCommand (uint8_t cmd)
{
	PORTB &=~(1<<RS);
	//PORTA =cmd;
    writeSr(cmd);
	enablePulse();
    //if(cmd==0x01 || cmd ==0x02)
	if(cmd == 0b00000001 || cmd ==0b00000010)
    {
      _delay_us(2000); 
    }
    else{
      _delay_us(100);
    }

}



void writeData(uint8_t data)
{
	PORTB |=(1<<RS);
	//PORTA =data;
    writeSr(data);
	enablePulse();
	_delay_us(100);

}

void init_all()
{
     DDRA |=(1<< clockPin | 1<< latchPin| 1<< dataPin); ///configuring as output pin    
     DDRB |=0xff;
     PORTA|=0x00;
     PORTB|=0x00;
     PORTB &=~(1<<clrpin);
     PORTB |=(1<<clrpin);

     ////8 bit mode
	//writeCommand(0x0E);//14
    writeCommand(0b00001110);
	////LCD ON CURSOR ON CURSOR BLINKING ON
	//writeCommand(0X0F);//15
    writeCommand(0b00001111);
	////LCD CURSOR POSITION
	//writeCommand(0X1C);//28
    writeCommand(0b00011100);
	////CLEARING THE DISPLAY
	//writeCommand(0x01);//1
    writeCommand(0b00000001);
}

int main()
{
    init_all();
    _delay_ms(100);
    
  // writeData(0b1010000);
  // writeData(0b1010010);
  // writeData(0b1000101);
  // writeData(0b1010011);
  // writeData(0b1010011);
  // writeData(0b1010101);
  // writeData(0b1010010);
  //writeData(0b1000101);

    return 0;
}


/////VOID HIGH() AND VOID LOW() THIS FUNCTION IS AS AN SERIAL INPUT PIN 




