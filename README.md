# IBM PC XT compatible (2022)
This is a IBM XT compatible built from scratch along 8 boards, 6 of them ISA 8-bit cards.


The design of the boards done here, is a bit personal. As other pcbs i do in-house, they are projected to be transfered with iron, drilled, then etched with iron perchlorate, using thought-hole chips, as many electronic and microcomputer hobbyists once did. To minimize errors on these manual procedures, circuits tracks and vias are made thicker. This project is about creating an IBM XT Compatible. This began in 2014, when i started to documenting home computer systems, like older and newer PCs, another older systems like ZX Spectrum, MSX, Commodore, Amiga, Apple II, Macintosh, home and portable Sega, Nintendo and Sony entretainment systems, mainly for coding emulators. The IBM PC XT, was the second architecture to fully take notes after the ZX Spectrum.
In 2021, after manually designing and building an Apple 2 compatible computer and an Adlib card, i ask my self "it would be possible to build a bigger system an IBM PC XT compatible using the same methods and tools???" (it seemed a big task by then). When i began, i wanted to be 8086 compatible, but the 16-bit bus and 16-bit memory organization logic, would make it impossible to fit in the maximum dimensions allowed by my own tools (A4 copper sheets), further the capacitance and thus distortion would be more pronounced on circuits.
Most components on IBM PC XT are Intel, DRAM, ROM ICs. Some control, demuxes and muxes, buffers, latches, and other glue logic is done using 74 series.


**References**
  * The Indispensable PC Hardware Book - Hans Peter Messmer; 1994
  * Digital Systems Project / Projecto de Sistemas Digitais (ISEL) - Victor Pimenta Rodrigues, Mário Seia de Araujo; 1988 (3th edition 2001)
  * IBM PC XT Technical Reference; 1983
  * Electrónica AC/DC/Analise circuitos - Santos, Guimarães e Oliveira, SA. Porto; 1980s
  * Interfacing to the IBM Personal Computer; Lewis C. Eggebrecht; 1983
  * Vintage Computer Federation (VCF)
  * GALBlast by Manfred Winterhoff
  * AT2XT Keyboard Adapter by Charles Guzis
  * XT-IDE by Charles Guzis, Vintage Computer Federation member (VCF)
  * HelpPC Reference Library by David Jurgens
  * Every datasheet you can think of.

**Thanks to people i actually e-mailed them...**
  - CFG register will set the GALs scheme? Manfred Winterhoff, the author of GALBlast project
  - Can HTC lag on XT-IDE? Charles Guzis, the author of XT-IDE, XT2PS2 project
  - Whats the use of this diode on Adlib? the author of an Adlib, Sergey Malinov 
