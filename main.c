/*
 * main.c
 *
 *  Created on: May 9, 2012
 *      Author: bgouveia
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include "usiTwiSlave.h"
#include "ADC_tiny.h"



enum {READ, SETSENSIVITY};

#define SLAVEADDRESS 0x26



/* Brief: The main function.
 * The program entry point. Initates TWI and enters eternal loop, waiting for data.
 */
int main(void)
{
	//OSCCAL=51; //set to 12MHZ;


	DDRB &= ~((1<<DDB3)|(1<<DDB4) |(1<<DDB5));        // pb3,4,5 set up as input
	DDRB |= (1<<DDB1);		// Set pb1 as output


	uint8_t sens = eeprom_read_byte((uint8_t*)0);


	if (sens == 6){
		PORTB |= (1<<PB1);
	}else{
		PORTB &= ~(1<<PB1);
	}

	bool reading = true;


	usiTwiSlaveInit(SLAVEADDRESS);

	initADC();


	sei();

	// This loop runs forever. If the TWI Transceiver is busy the execution will
	// just continue doing other operations.
	for(;;)
	{


		if(usiTwiDataInReceiveBuffer())
		{
			uint8_t temp = usiTwiReceiveByte();

			if (reading){
				uint16_t value=0;
					switch (temp)
					{
						case READ:
							value = readADCBlocking(2);
							readADC(3);
							usiTwiTransmitByte(value>>8);
							usiTwiTransmitByte(value&0xFF);
							value = getADC();
							readADC(0);
							usiTwiTransmitByte(value>>8);
							usiTwiTransmitByte(value&0xFF);
							value = getADC();
							usiTwiTransmitByte(value>>8);
							usiTwiTransmitByte(value&0xFF);
							break;

						case SETSENSIVITY :
							reading=false;
							break;

					}
			}else{
					switch(temp){
						case 2:
							PORTB &= ~(1<<PB1);
							eeprom_write_byte((uint8_t*)0,2);
							break;
						case 6:
							PORTB |= (1<<PB1);
							eeprom_write_byte((uint8_t*)0,6);
							break;
					}
					reading=true;
			}

		}

		// Do something else while waiting for the TWI transceiver to complete.

	}
}
