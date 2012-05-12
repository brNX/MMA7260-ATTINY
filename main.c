/*
 * main.c
 *
 *  Created on: May 9, 2012
 *      Author: bgouveia
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "usiTwiSlave.h"
#include "ADC_tiny.h"

#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))



uint16_t value;
uint16_t analog;

/* Brief: This section determines what format to return the value in. */

enum { DATA1, DATA2, RAW };

//-----------------------------------------------------------------------------

/* This section allows you to select between three different modes. In this sample
each return the value of the analog at different levels just to give an idea of how
it can be used. */

inline void mode(uint8_t mode, uint8_t on)
{
	if (on){
		value = readADC(mode);
		PORTB ^= (1<<PB1);
	}

}

/* Brief: The main function.
 * The program entry point. Initates TWI and enters eternal loop, waiting for data.
 */
int main(void)
{
	//OSCCAL=51; //set to 12MHZ;


	unsigned char slaveAddress, temp;

	//DDRB &= ~(1<<DDB1);        // pb1 set up as input
	DDRB &= ~(1<<DDB3);        // pb3 set up as input
	DDRB |= (1<<DDB1);		// Set pb1 as output


	slaveAddress = 0x26;		// This can be change to your own address
	usiTwiSlaveInit(slaveAddress);

	initADC();
	_delay_ms(10);


	sei();

	// This loop runs forever. If the TWI Transceiver is busy the execution will
	// just continue doing other operations.
	for(;;)
	{


		if(usiTwiDataInReceiveBuffer())
		{
			temp = usiTwiReceiveByte();
			switch (temp)
			{
			case 1: mode(1, 1);   break;		// the case is selected by a single
			case 2: mode(2, 1);   break;		// digit in the master code. (1,2 or 3)
			case 3: mode(3, 1);   break;
			}

			usiTwiTransmitByte(value>>8);
			usiTwiTransmitByte(value&0xFF);
		}

		// Do something else while waiting for the TWI transceiver to complete.

	}
}
