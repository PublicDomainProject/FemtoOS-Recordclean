/*
 * Femto OS v 0.92 - Copyright (C) 2008-2010 Ruud Vlaming
 *
 * This file is part of the Femto OS distribution.
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
 * licensed. A commercial license and support are available.
 * See http://www.femtoos.org/ for details.
 */

/**
 * This file is the main source for the microcontroller used in the 
 * record cleaning machine built for the Public Domain Project
 * (http://de.publicdomainproject.org/index.php/PD_Diskussion:Gemeinschaftsportal/Reinigungsprozess#Plattenwaschmaschine)
 * 
 * The processor is a ATMEGA 8535 from Atmel mounted on a 
 * AVR-P40-8535-8MHz Protoboard from Olimex
 * (https://www.olimex.com/Products/AVR/Proto/AVR-P40-8535-8MHz/)
 * 
 * This firmware is using the preemtive realtime operating systems
 * FemtoOS which is also licensed under GPLv3 
 * (http://femtoos.org/index.html) 
 *
 * Time for one tick: 260 µs
 * 
 * Programm this code with following FUSE bits:
 *
 * Lower Fusebits:  0x84
 * Higher Fusebits: 0xD9
 *
 */


/* This this the only include needed to use the Femto OS.*/
#include "femtoos_code.h"

#include "recordclean.h"

#ifdef DEBUG
#include "femtoos_libuart.h"
#endif

#include <stdlib.h>

/****************************************************************************/
/* Global Variables used by more than one task                              */
/****************************************************************************/
static Tuint08 ChangedTangentialSpeed = 0;
static Tuint08 Servo = 0;
static Tbool   ServoRun = false;
static Tuint08 PulseTime = 0;

static


/****************************************************************************/
/* Function definitions                                                     */
/****************************************************************************/

/* Initialize EEPROM registers */
void initEEPROM()
{
	EEAR = 0;
	EECR = 0;
	EEDR = 0;
}

/* Taken from the Atmel Datasheet */
Tuint08 readEEPROM(Tuint16 Address)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
		;
	/* Set up Address Register */
	EEAR = Address;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}

#ifdef DEBUG
const Tchar wroteToEEStr[] PROGMEM = "Wrote to EEPROM";
#endif

/* Taken from the Atmel Datasheet */
void writeEEPROM(Tuint16 Address, Tuint08 Data)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEWE))
		;
	/* Set up Address and Data Registers */
	EEAR = Address;
	EEDR = Data;
	taskEnterGlobalCritical();
	/* Write logical one to EEMWE */
	EECR |= (1<<EEMWE);
	/* Start EEPROM write by setting EEWE */
	EECR |= (1<<EEWE);
	taskExitGlobalCritical();

	#ifdef DEBUG
	libuart_puts((Tchar*)wroteToEEStr);
	libuart_sendraw('\n');
	#endif
}


void initFreqGenTimer()
{
	/* If you use a different AVR type, copy this part and change the settings: */
	#if defined(__AVR_ATmega8535__)

	/* Prescaler set to /8
	 * Timer mode set to Clear Timer on Compare	Match (CTC)
	 * OC2 output set to toggle (Toggles everytime the compare matches) */
	TCCR2 = 0 | (1<<WGM21) | (0<<WGM20) | (0<<COM21) | (1<<COM20) | devPrescale_8;

	// Initial value gives a frequency of 16 kHz
	// 30 is correct for 45 rpm
	// 21 is correct for 33.3 rpm
	OCR2  = 21;

	#endif

}


void initPwmUnit()
{
	/* If you use a different AVR type, copy this part and change the settings: */
	#if defined(__AVR_ATmega8535__)

	/* Clock Select set to: I/O Clock divided  by 8
	 * Timer mode set to:   PWM, phase and frequency correct, TOP = ICR1 (Mode 8)
	 * OC1A output set to:  non-inverted PWM mode (Mode 2)*/
	TCCR1A = 0 | (0<<WGM11)  | (0<<WGM10)
			   | (1<<COM1A1) | (0<<COM1A0)
			   | (1<<COM1B1) | (0<<COM1B0);

	TCCR1B = 0 | (1<<WGM13) | (0<<WGM12) | devPrescale_1;

	/* Initial values, stopped motor (breaking) */
	ICR1  = 8192; //492.5 Hz
	OCR1A = 0;    // aussen
	OCR1B = 0;    // innen

	#endif

}

void armMotorInward(Tuint16 speed)
{
	OCR1A = 0;
	OCR1B = ((Tuint16)speed);
}

void armMotorOutward(Tuint16 speed)
{
	OCR1A = ((Tuint16)speed);
	OCR1B = 0;
}

void armMotorStop()
{
	OCR1A = 0;
	OCR1B = 0;
}


/****************************************************************************/
/* FemtoOS specific call-backs                                              */
/****************************************************************************/

void appBoot(void)
{
	#ifdef DEBUG
		libUartInit();
	#endif

	initLed();
	initTurntable();
	initPump();
	initNozzle();
	initServo();

	initPushButtons();
	initLimitSwitches();

	initFreqGenTimer();
	initFreqGenPort();

	initPwmUnit();
	initArmMotorPort();

}

void appTick00(void)
{
	Servo++;

	if(Servo <= PulseTime) {
		setServo(1);
	}
	else if(Servo >= PERIODETIME) {
		Servo = 0;
	}
	else {
		setServo(0);
	}
}

/****************************************************************************/
/* FemtoOS tasks                                                            */
/****************************************************************************/

#if (preTaskDefined(Background))

void appLoop_Background(void)
{
	Tuint08 Live = 0;

	while (true)
	{
		/* This does blink the Live LED */
		setLiveLed(Live);
		if(Live == 0)
			Live = 1;
		else
			Live = 0;

		/* Check if channel setting is changed and in case store it */
		if(0 < ChangedTangentialSpeed) {
			if(appEEPROMWriteDelay > ChangedTangentialSpeed)
				ChangedTangentialSpeed++;
			else {
				writeEEPROM(appEETangentialSpeed, 5);
				ChangedTangentialSpeed = 0;
				#ifdef DEBUG
					EEDR = 0;
					libuart_sendraw((readEEPROM(appEETangentialSpeed)+'0'));
					libuart_sendraw('\n');
				#endif
			}
		}
		else {
			/* Set EEPROM address to a not used value to avoid unintended writes */
			EEAR = 0;
		}

		taskDelayFromWake(2000);
	}
}

#endif


/* This task does the handling of the servo */
#if (preTaskDefined(Servo))

void appLoop_Servo(void)
{
	Tuint16 WaitCounter = 0;

	PulseTime = RUNPULSETIME;

	while (true)
	{

		if(ServoRun != false) {
			// From time to time, unspool some thread
			if(0 == WaitCounter) {
				WaitCounter = 100;
				PulseTime = RUNPULSETIME;
			}
			else {
				WaitCounter--;
				PulseTime = STOPPULSETIME;
			}
		}
		else {
			WaitCounter = 0;
			PulseTime = STOPPULSETIME;
		}

		taskDelayFromNow(500);
	}
}
#endif

/* This task is the main task and the state-machine here controls the whole process.
 * It handles all inputs like buttons and limit switches and also the simple outputs
 * like vacuum pump and Nozzle magnet
 */
#if (preTaskDefined(RecordClean))

void appLoop_RecordClean(void)
{
	const Tuint16 TASKDELAYTIME  = 500;
	const Tuint08 NOZZLEWAITTIME = 5000/TASKDELAYTIME;
	const Tuint08 DEBOUNCETIME   = 1500/TASKDELAYTIME;

	Tmachinestate MachineState = Setup;

	Tuint08 PushButton  = 0;
	Tuint08 LimitSwitch = 0;

	Tuint08 PushButtonUpDownEvent = 0;

	Tuint16 WaitTimeCounter = 0;

	while (true)
	{
		/* Check if a button was pressed */
		if(PushButton != getPushButtons())
		{
			if((PushButton & devPushButtonUpDown) != 0
				&& (getPushButtons() & devPushButtonUpDown) == 0) {
				PushButtonUpDownEvent = 1;
			}
			else
				PushButtonUpDownEvent = 0;

			PushButton = getPushButtons();
		}

		/* Check if a limit switch was activated */
		if(LimitSwitch != getLimitSwitches())
		{
			LimitSwitch = getLimitSwitches();
		}

		/* State machine to control the cleaning process */
		switch(MachineState)
		{
			case Setup:
				setTurntable(0);
				setPump(0);
				setNozzle(0);
				armMotorOutward(0xFFFF);
				ServoRun = false;

				WaitTimeCounter = 0;

				if((LimitSwitch & devLimitSwitchOutside) == 0){
					MachineState = Idle;
				}
				break;
			case Idle:
				setTurntable(0);
				setPump(0);
				setNozzle(0);
				armMotorStop();
				ServoRun = false;

				/* Wait some time to debounce the switches */
				if(WaitTimeCounter != DEBOUNCETIME)
				{
					WaitTimeCounter++;
				}

				if((LimitSwitch & devLimitSwitchArm) != 0){
					WaitTimeCounter = 0;
					MachineState = PrepareForCleaning;
				}

				if(((PushButton & devPushButtonUpDown) == 0  ||
				    (PushButton & devPushButtonFaster) == 0  ||
				    (PushButton & devPushButtonSlower) == 0) &&
				   WaitTimeCounter == DEBOUNCETIME){
					WaitTimeCounter = 0;
					MachineState = TurntableOn;
				}
				break;
			case TurntableOn:
				setTurntable(1);
				setPump(0);
				setNozzle(0);
				armMotorStop();
				ServoRun = false;

				/* Wait some time to debounce the switches */
				if(WaitTimeCounter != DEBOUNCETIME)
				{
					WaitTimeCounter++;
				}

				if((LimitSwitch & devLimitSwitchArm) != 0){
					WaitTimeCounter = 0;
					MachineState = PrepareForCleaning;
				}

				if(((PushButton & devPushButtonUpDown) == 0  ||
				    (PushButton & devPushButtonFaster) == 0  ||
				    (PushButton & devPushButtonSlower) == 0) &&
				   WaitTimeCounter == DEBOUNCETIME){
					WaitTimeCounter = 0;
					MachineState = Idle;
				}
				break;
			case PrepareForCleaning:
				setTurntable(1);
				setPump(1);
				setNozzle(0);
				armMotorInward(0xFFFF);
				ServoRun = false;

				/* Wait some time to debounce the arm switch */
				if(WaitTimeCounter != DEBOUNCETIME)
				{
					WaitTimeCounter++;
				}

				if((LimitSwitch & devLimitSwitchInside) == 0){
					MachineState = NozzleDown;
					WaitTimeCounter = 0;
					armMotorStop();
				}

				if((LimitSwitch & devLimitSwitchArm) == 0 && WaitTimeCounter == DEBOUNCETIME){
					MachineState = Setup;
					WaitTimeCounter = 0;
				}
				break;
			case NozzleDown:
				setTurntable(1);
				setPump(1);
				setNozzle(1);
				armMotorStop();
				ServoRun = true;

				/* Wait some time until nozzle is completely lowered */
				if(WaitTimeCounter != NOZZLEWAITTIME)
				{
					WaitTimeCounter++;
				}
				else
				{
					MachineState = Cleaning;
					WaitTimeCounter = 0;
				}

				if(PushButtonUpDownEvent != 0 && WaitTimeCounter > NOZZLEWAITTIME){
					/* The WaitTimeCounter is used to avoid fast toggling of states */
					MachineState = NozzleUpStop;
					WaitTimeCounter = 0;
				}

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Setup;
					WaitTimeCounter = 0;
				}
				break;
			case Cleaning:
				setTurntable(1);
				setPump(1);
				setNozzle(1);
				ServoRun = true;

				WaitTimeCounter = 0;

				/* TODO: adjust motor speed while moving outwards.
				 * We could move faster on the inner circles and would have
				 *  to slow down when getting further away from the center */
				armMotorStop(0);
				MachineState = CleaningMove;

				if((LimitSwitch & devLimitSwitchOutside) == 0){
					armMotorStop();
					MachineState = WaitForArmLifting;
				}

				if((PushButton & devPushButtonUpDown) == 0){
					armMotorStop();
					MachineState = NozzleUpStop;
				}

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Setup;
				}

				if((PushButton & devPushButtonFaster) == 0){

				}
				if((PushButton & devPushButtonSlower) == 0){

				}
				break;
			case CleaningMove:
				setTurntable(1);
				setPump(1);
				setNozzle(1);
				ServoRun = true;

				WaitTimeCounter = 0;

				/* TODO: adjust motor speed while moving outwards.
				 * We could move faster on the inner circles and would have
				 *  to slow down when getting further away from the center */
				armMotorOutward(900);
				MachineState = Cleaning;

				if((LimitSwitch & devLimitSwitchOutside) == 0){
					armMotorStop();
					MachineState = WaitForArmLifting;
				}

				if((PushButton & devPushButtonUpDown) == 0){
					armMotorStop();
					MachineState = NozzleUpStop;
					}

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Setup;
				}

				if((PushButton & devPushButtonFaster) == 0){

				}
				if((PushButton & devPushButtonSlower) == 0){

				}
				break;
			case NozzleUpStop:
				setTurntable(1);
				setPump(1);
				setNozzle(0);
				armMotorStop();
				ServoRun = false;

				/* Wait some time until nozzle is completely lifted */
				if(WaitTimeCounter != DEBOUNCETIME)
				{
					WaitTimeCounter++;
				}

				if(PushButtonUpDownEvent != 0 && WaitTimeCounter == DEBOUNCETIME){
					MachineState = NozzleDown;
					WaitTimeCounter = 0;
				}

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Setup;
					WaitTimeCounter = 0;
				}

				if((PushButton & devPushButtonFaster) == 0){
					MachineState = NozzleUpOutward;
				}
				if((PushButton & devPushButtonSlower) == 0){
					MachineState = NozzleUpInward;
				}
				break;
			case NozzleUpInward:
				setTurntable(1);
				setPump(1);
				setNozzle(0);
				ServoRun = false;

				if((LimitSwitch & devLimitSwitchInside) != 0)
					armMotorInward(1600);
				else
					armMotorStop();

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Setup;
				}
				else if((PushButton & devPushButtonSlower) == 0){
					MachineState = NozzleUpInward;
				}
				else
					MachineState = NozzleUpStop;
				break;
			case NozzleUpOutward:
				setTurntable(1);
				setPump(1);
				setNozzle(0);
				ServoRun = false;

				if((LimitSwitch & devLimitSwitchOutside) != 0)
					armMotorOutward(1600);
				else
					armMotorStop();

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Setup;
				}
				else if((PushButton & devPushButtonFaster) == 0){
					MachineState = NozzleUpOutward;
				}
				else
					MachineState = NozzleUpStop;
				break;
			case WaitForArmLifting:
				/* Cleaning has ended, we have to wait until the user lifts the arm */
				setTurntable(0);
				setPump(0);
				setNozzle(0);
				armMotorStop();
				ServoRun = false;

				WaitTimeCounter = 0;

				if((LimitSwitch & devLimitSwitchArm) == 0){
					MachineState = Idle;
					WaitTimeCounter = 0;
				}
				break;
			default:
				setTurntable(0);
				setPump(0);
				setNozzle(0);
				ServoRun = false;
				WaitTimeCounter = 0;
				MachineState = Idle;
				break;
		}

		taskDelayFromNow(TASKDELAYTIME);
	}
}
#endif

