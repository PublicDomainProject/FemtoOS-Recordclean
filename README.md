# FemtoOS-Recordclean
This file is the source code for the microcontroller used in our record cleaning machine

This file is the main source for the microcontroller used in the
record cleaning machine built for the Public Domain Project.

Photos of the machine an the mechanics can be found in the Public Domain Project Wiki:
http://de.publicdomainproject.org/index.php/PD_Diskussion:Gemeinschaftsportal/Reinigungsprozess#Plattenwaschmaschine

Hardware
--------

The processor is a ATMEGA 8535 from Atmel mounted on a
AVR-P40-8535-8MHz Protoboard from Olimex
(https://www.olimex.com/Products/AVR/Proto/AVR-P40-8535-8MHz/)

FemtoOS
-------
This firmware is using the preemtive real-time operating systems
FemtoOS which is also licensed under GPLv3
(http://femtoos.org/index.html)

Time for one tick: 260 µs

Version used: 0.922

Development
-----------

This is a Eclipse project using the AVR Eclipse Plugin. The project file is included in the repository (as usual for Eclipse it is a hidden file).


Compilation
-----------

FemtoOS does not build currently with newer gcc's (one guy is working on patches for this).

Currently this source is built with the following tool versions:

* avr-libc 1.6.8
* gcc-avr 4.3.5
* binutils-avr 2.24

Fuse bits
---------

Program this code with following FUSE bits:

Lower Fusebits:  0x84
Higher Fusebits: 0xD9
