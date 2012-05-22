/*
 * ADC_tiny.h
 *
 *  Created on: May 11, 2012
 *      Author: bgouveia
 */

#ifndef ADC_TINY_H_
#define ADC_TINY_H_

#include <avr/io.h>

inline void initADC()
{

	//ADMUX &= ~ (1<<REFS0 | 1<<REFS1 | 1<<REFS2); //set Vref = Vcc  ,
	//ADMUX =_BV(REFS1); //set to 1.1V internal for temperature (_BV(REFS1))
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);
}

inline uint16_t getADC(){
	while ( ADCSRA & ( 1 << ADSC ));

	return (ADCL | (ADCH << 8));
}

inline uint16_t readADCBlocking (uint8_t channel)
{
	ADMUX = (ADMUX&0xF0) | channel ;

	ADCSRA |= (1<<ADSC);

    return getADC();
}

inline void readADC (uint8_t channel)
{
	ADMUX = (ADMUX&0xF0) | channel ;

	ADCSRA |= (1<<ADSC);
}



#endif /* ADC_TINY_H_ */
