# FemtoOS-Recordclean
This file is the source code for the microcontroller used in our record cleaning machine

This file is the main source for the microcontroller used in the
record cleaning machine built for the Public Domain Project.

Fotos of the machine an the mechanics can be found in the Public Domain Project Wiki:
http://de.publicdomainproject.org/index.php/PD_Diskussion:Gemeinschaftsportal/Reinigungsprozess#Plattenwaschmaschine

The processor is a ATMEGA 8535 from Atmel mounted on a
AVR-P40-8535-8MHz Protoboard from Olimex
(https://www.olimex.com/Products/AVR/Proto/AVR-P40-8535-8MHz/)

This firmware is using the preemtive realtime operating systems
FemtoOS which is also licensed under GPLv3
(http://femtoos.org/index.html)

Time for one tick: 260 µs

Programm this code with following FUSE bits:

Lower Fusebits:  0x84
Higher Fusebits: 0xD9
