/*
 * femtoos_libuart.c
 *
 *  Created on: 08.07.2010
 *      Author: nuess0r, based on code from drgry
 *
 * This library works together with the Femto OS distribution.
 *
 *
 * You have to adjust the following config parameters in "config_application.h":
 *
 * SYSTEM CONFIGURATION ====================================================
 * #define  cfgSysSqueezeState                      cfgTrue
 * --> or keep the default value and change two defines in your device asm file
 *     you MUST do it this way if you like to use events:
 * #define devAuxEventReg    UBRRL <-- change this register to an unused one
 * #define devAuxSysReg      UBRRL
 *
 * INTERRUPT HANDLING ======================================================
 * #define  cfgIntUserDefined                       cfgTrue
 *
 *
 * ISR header/footer functions ---------------------------------------------
 * #define  includeIsrEnter                         cfgTrue
 * #define  includeIsrExit                          cfgTrue
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Please note that, due to the GPLv3 license, for application of this
 * work and/or combined work in embedded systems special obligations apply.
 * If these are not to you liking, please know the Femto OS is dual
 * licensed.
 * See http://www.femtoos.org/ for details.
 */

#include "femtoos_code.h"

#ifdef USE_LIBUART

#include "femtoos_libuart.h"

#ifndef F_CPU
#warning "F_CPU not defined, default value used. Set it with the compiler options. For avr-gcc: -DF_CPU=1000000ULL"
#define F_CPU 1000000ULL
#endif

/* ( (I/O clock frequency)/ baudrate / 16) - 1  */
#define BAUDRATE_COUNT_VAL ((F_CPU / BAUDRATE / 16UL) -1UL)


/* Global variables for message buffer and read/write indexes */
/** Transmit buffer write pointer */
static volatile Tuint08 libuart_txIndexIn;
/** Transmit butter read pointer */
static volatile Tuint08 libuart_txIndexOut;

/** Transmit buffer */
static volatile Tchar libuart_txBuffer[64];
#define libuart_txIndexMask 0x3F

#ifdef LIBUART_RECEIVE
/** Receive buffer write pointer */
static volatile Tuint08 libuart_rxIndexIn;
/** Receive buffer read pointer */
static volatile Tuint08 libuart_rxIndexOut;

/** Receive buffer */
static volatile Tchar libuart_rxBuffer[64];
#define libuart_rxIndexMask 0x3F

/* ISR to read received data from the UART */
void USART_RXC_vect(void) __attribute__ ( ( signal, naked, used, externally_visible ) );
void USART_RXC_vect(void){
	isrEnter();
	//genQueuWriteOnName(UARTRx,UDR);
	isrExit();
}
#endif /* LIBUART_RECEIVE */

/* ISR to write data to be sent to the UART */
void USART_UDRE_vect(void) __attribute__ ( ( signal, naked, used, externally_visible ) );
void USART_UDRE_vect(void){
	isrEnter();

	UDR = libuart_txBuffer[(libuart_txIndexOut & libuart_txIndexMask)];
	libuart_txIndexOut++;

	/* If there is nothing to send left, deactivate UART transfer register empty interrupt */
	if ((libuart_txIndexIn & libuart_txIndexMask) == (libuart_txIndexOut & libuart_txIndexMask))
	{
		UCSRB &= ~(1<<UDRIE);
	}
	isrExit();
}

/* UART and task initialization. Call this function in appBoot() */
void libUartInit(void)
{

	UBRRL = (Tchar) BAUDRATE_COUNT_VAL; // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UBRRH = (Tchar)(BAUDRATE_COUNT_VAL >> 8); // Load upper 8-bits of the baud rate value into the high byte of the UBRR register

	libuart_txIndexIn  = 0;
	libuart_txIndexOut = 0;

#ifdef LIBUART_RECEIVE
	libuart_rxIndexIn  = 0;
	libuart_rxIndexOut = 0;
	UCSRB = (1 << RXEN) | (1 << TXEN) | (1 << RXCIE);   // Turn on the transmission and reception circuitry
#else
	UCSRB = (1 << TXEN);   // Turn on the transmission and reception circuitry
#endif

	UCSRC = (UCSRC | (0b10000110));
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes, 1 Stop bit (Bit 3 = 0)
}


void libuart_puts(Tchar* string)
{
	Tuint08 i = 0;
	Tchar c;

	do
	{
		c = portFlashReadByte(Tchar, string[i++]);
		libuart_txBuffer[(libuart_txIndexIn & libuart_txIndexMask)] = c;
		libuart_txIndexIn++;
	} while ('\0' != c);

	libuart_txIndexIn--;

	UCSRB |= (1 << UDRIE);
}

void libuart_sendraw(Tuint08 rawbyte)
{
	libuart_txBuffer[(libuart_txIndexIn & libuart_txIndexMask)] = rawbyte;
	libuart_txIndexIn++;
	UCSRB |= (1 << UDRIE);
}
#endif
