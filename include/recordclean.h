/*
 * tube_amp.h
 *
 *  Created on: 05.08.2010
 *      Author: chrigi
 */

#ifndef TUBE_AMP_H_
#define TUBE_AMP_H_

#include "femtoos_code.h"


/* select here the desired washing machine model to get the correct pinning: */
#define REVOX_B795

/*--------------------------------------------------------------------------*/
#ifdef REVOX_B795

//#define DEBUG

/* Pinning for the live LED */
#define devLiveLedPort  PORTB
#define devLiveLedDDR   DDRB
#define devLiveLed 0
#define devLiveLedMask  ((1 << devLiveLed))

/* Initialize LED */
#define initLed() { devLiveLedPort &= ~devLiveLedMask; \
   				    devLiveLedDDR |= devLiveLedMask;   }

#define setLiveLed(out) (devLiveLedPort = ((devLiveLedPort & ~devLiveLedMask ) | out))

/* Pinning for the turntable enable out*/
#define devTurntablePort  PORTB
#define devTurntableDDR   DDRB
#define devTurntable 1
#define devTurntableMask  ((1 << devTurntable))

/* Initialize  turntable enable out*/
#define initTurntable() { devTurntablePort &= ~devTurntableMask; \
   				          devTurntableDDR |= devTurntableMask;   }

#define setTurntable(out) (devTurntablePort = ((devTurntablePort & ~devTurntableMask ) | (out << devTurntable)))

/* Pinning for the pump enable out*/
#define devPumpPort  PORTB
#define devPumpDDR   DDRB
#define devPump 2
#define devPumpMask  ((1 << devPump))

/* Initialize  pump enable out*/
#define initPump() { devPumpPort &= ~devPumpMask; \
   				     devPumpDDR |= devPumpMask;   }

#define setPump(out) (devPumpPort = ((devPumpPort & ~devPumpMask ) | (out << devPump)))

/* Pinning for the nozzle down out*/
#define devNozzlePort  PORTB
#define devNozzleDDR   DDRB
#define devNozzle 3
#define devNozzleMask  ((1 << devNozzle))

/* Initialize  nozzle down out*/
#define initNozzle() { devNozzlePort &= ~devNozzleMask; \
   				       devNozzleDDR |= devNozzleMask;   }

#define setNozzle(out) (devNozzlePort = ((devNozzlePort & ~devNozzleMask ) | (out << devNozzle)))

/* Pinning for the servo out (Thread spool motor) */
#define devServoPort  PORTB
#define devServoDDR   DDRB
#define devServo 6
#define devServoMask  ((1 << devServo))

/* Initialize servo out */
#define initServo() { devServoPort &= ~devServoMask; \
   				      devServoDDR |= devServoMask;   }

#define setServo(out) (devServoPort = ((devServoPort & ~devServoMask ) | (out << devServo)))

/* Pinning for the push buttons */
#define devPushButton0InPort     PORTD
#define devPushButton0In         PIND
#define devPushButton0InDDR      DDRD
#define devPushButton0InMask     ((1 << 2) | (1 << 3))
#define devPushButton0InDefault  ((1 << 2) | (1 << 3))	  /* activate internal pull-up resistors */
#define devPushButton1InPort     PORTD
#define devPushButton1In         PIND
#define devPushButton1InDDR      DDRD
#define devPushButton1InMask     (1 << 6)
#define devPushButton1InDefault  (1 << 6)	  /*activate internal pull-up resistors */

#define initPushButtons() {	devPushButton0InPort |= devPushButton0InDefault; \
							devPushButton0InDDR  &= ~devPushButton0InMask;   \
							devPushButton1InPort |= devPushButton1InDefault; \
							devPushButton1InDDR  &= ~devPushButton1InMask;   }
#define getPushButtons() (((devPushButton1In & devPushButton1InMask) >> 4) | ((devPushButton0In & devPushButton0InMask) >> 2))

#define devPushButtonFaster	     (1 << 0)
#define devPushButtonSlower	     (1 << 1)
#define devPushButtonUpDown	     (1 << 2)

/* Pinning for the Limit Switches */
#define devLimitSwitchesInPort     PORTA
#define devLimitSwitchesIn         PINA
#define devLimitSwitchesInDDR      DDRA
#define devLimitSwitchesInMask     ((1 << 0) | (1 << 1) | (1 << 2))
#define devLimitSwitchesInDefault  ((1 << 0) | (1 << 1) | (1 << 2))  /*activate internal pull-up resistors */

#define initLimitSwitches() {devLimitSwitchesInPort |= devLimitSwitchesInDefault; \
							 devLimitSwitchesInDDR  &= ~devLimitSwitchesInMask;   }
#define getLimitSwitches()  ((devLimitSwitchesIn & devLimitSwitchesInMask))

#define devLimitSwitchOutside      (1 << 0)
#define devLimitSwitchInside       (1 << 1)
#define devLimitSwitchArm          (1 << 2)

/* Pinning for the Frequency Generator Output */
#define devFreqGenPort     PORTD
#define devFreqGenDDR      DDRD
#define devFreqGenOutMask  (1 << 7)
#define devFreqGenDefault  (1 << 7)	  /*set to output */

#define initFreqGenPort() {devFreqGenDDR |= devFreqGenDefault;}

/* Pinning for the arm motor */
#define devArmMotorPort     PORTD
#define devArmMotorDDR      DDRD
#define devArmMotorOutMask  ((1 << 4) | (1 << 5))
#define devArmMotorDefault  ((1 << 4) | (1 << 5)) /*set to output */

#define initArmMotorPort() {devArmMotorDDR |= devArmMotorDefault;}

#endif /* REVOX_B795 */
/*--------------------------------------------------------------------------*/


/* CPU specific stuff is defined here */
#if defined(__AVR_ATmega8535__)


#endif

/*--------------------------------------------------------------------------*/
/* General application specific stuff is defined here */
#define appEEPROMWriteDelay		6
#define appEETangentialSpeed    20

typedef enum {
	Setup, Idle, TurntableOn, PrepareForCleaning, NozzleDown, Cleaning,
	CleaningMove, NozzleUpStop, NozzleUpInward, NozzleUpOutward,
	WaitForArmLifting
} Tmachinestate;

/* Constants for the servo signal timing */
#define PERIODETIME   77
#define STOPPULSETIME 4
#define RUNPULSETIME  3

/*--------------------------------------------------------------------------*/

#endif /* VINYLWASH_H_ */
