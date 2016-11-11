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

#ifndef FEMTOOS_LIBUART_H_
#define FEMTOOS_LIBUART_H_

#include "femtoos_code.h"

#ifdef USE_LIBUART

#define BAUDRATE 19200UL

/* call this function inside your appBoot() function: */
void libUartInit(void);
void libuart_puts(Tchar* string);
void libuart_sendraw(Tuint08 rawbyte);
#endif

#endif /* FEMTOOS_LIBUART_H_ */
