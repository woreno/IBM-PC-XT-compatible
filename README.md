# IBM PC XT compatible (2022)
This is a IBM XT compatible built from scratch along 8 boards, 6 of them ISA 8-bit cards.


The design of the boards done here, is a bit personal. As other pcbs i do in-house, they are projected to be transfered with iron, drilled, then etched with iron perchlorate, using throught-hole chips, as many electronic and microcomputer hobbyists once did. To minimize errors on these manual procedures, circuits tracks and vias were made thicker. This project is about creating an IBM XT Compatible. This began in 2014, when i started to documenting home computer systems, like older and newer PCs, older systems like ZX Spectrum, MSX, Commodore, Amiga, Apple II, Macintosh, home and portable Sega, Nintendo and Sony entretainment systems, mainly for coding emulators. The IBM PC XT, was the second architecture to fully take notes after the ZX Spectrum.
In 2021, after manually designing and building an Apple 2 compatible computer and an Adlib card, i ask my self "it would be possible to build a bigger system like an IBM PC XT compatible using the same methods and tools???" (it seemed a big task by then). When i began, i wanted to be 8086 compatible, but this would make it impossible to fit in the maximum dimensions allowed by my own tools (A4 copper sheets), further this would increase the circuits capacitance, so i ended choosing the 8088.
Most components on IBM PC XT are Intel, DRAM, ROM ICs. Some control, demuxes and muxes, buffers, latches, and other glue logic is done using 74 series. There are row of jumpers that allows for instance, activate the debugger or changing the default text mode. 

**BIOS**

* Its compiled with TASM using DOSBOX.
* It performs a checksum at start.
* The next, most important BIOS step is to configure PIT and DMA controller chips so DRAM chips are refreshed.
* The BIOS was written from scratch except the INT10h (video services). The computer started without a videocard; first i built and relied on a ISA-card that displayed 4 seven-segment digits and 8 leds. Being the only way of displaying information from the computer, this board was very important, so after being built manually i tested rigorously the I/O with an ISA raiser, flipping buttons and multimeter, then, installing it on a Pentium PC testing it with C Code. The BIOS has slowly developed step by step using this display board, so there's code everywhere that makes I/O to this board (the BIOS will display step and errors by I/O to this board). After, i built the insane CGA card. At the first it used a primitive INT10h without a VBLANK wait (so a lot of white rain when printing), and at the end, since i was a bit exausted, i prefered to copy the original IBM PC XT, dissasembled INT10h code for having full compatiblity.
* After building the IDE/RTC card i had problems booting DOS. I had to checksum the blocks at INT13h and do a comparasion with the groundtruth in another PC. So i end up to include also Single-Step and Breakpoint features, and debugging code traces at INT13h that can be activated by code. At the end, the fault was some IC on the IDE/RTC card that wasnt working properly. 
* There's a CGA welcome screen (2 planes compacted with RLE) and digitilized voices for numerals (compacted with DeltaMod) these resources are glued and placed at 0xFB900. This boot screen displays how the jumpers are set on the motherboard and their function.
*  At the beggining at 0xF8000 there is the checksum and signature, and pointers to the previous resources. Theres a jump at 0xFFFF0 to the begining of the code. The digitalised voices were intended for announcing the time (with the RTC board) after POST, but at the end i decided to no call the feature, because it was too fancy.
* In the last revision of this computer, i added a switch to allow the PROMs to be written, and the BIOS can be updated via serial port, wifi or diskettes, so FAT code has added. Wifi was planned to added to the Paralllel/Serial/Joystick board, by adding a ESP8266 as the serial port (but i didn't finish this part); Wifi settings are stored in RTC memory along other BIOS settings. The make script composes a package to that goal.
* Finally, there's a setup screen accessible with function key press at startup that allows to setup devices, and more - for instance setting a COM I/O port or the device boot order. If the IDE/RTC board is present these values are drawn from/saved to the RTC memory (it performs a checkup first), otherwise the values defined in this screen are just one-time.


**The Serial/Parallel/Joystick board**

  This is basically 3 IBM cards all in one, using the same 74 discrete logic.

**The IDE/RTC board**

  This is a card built from a similar IDE project from Vintage Computer Federation. It adds a RTC chip from DS12887 Dallas integrating a crystal oscillator and battery; there's some additional discrete logic as this chip wasn't directly interfaceable with I/O (it has an additional AS line, refer to the manual). I needed to fix the motherboard to make an exception to allow ISA cards to use I/O ports 0x70/1, thus a RTC card is able to use this legacy I/O port address. Since the Dallas chip's battery drained just within a few days of testing, i had to drill the enclosure and add a second battery (this seemed the usual practice in the retrocomputing community).

**The Floppy Disk board**

  This is basically a PS/2-era floppy disk controller chip from Intel, the 82077. Before this chip, FDC boards required a few chips, so this is a first generation of all-in-one FDC chips. This board adds another ISA slot, because i wanted to try an Ethernet card, and at end, i ran out of slots.

**The features I/O test board**
 The features I/O test board has 8-leds + 4 digits 7-segment display (and a Texas Instruments DM9368 for displaying hexadecimal) + ADC Intel ADC0808 Chip (for reading voltages); the 8-leds, 4 digits 7-segment display and knobs can be installed on a front panel and connected to the board with ribbon cables. All the features on this card are accessed with I/O ports - the 8 leds and 4 digits display are set with latches, the ADC values (2 channels) comes directly from the ADC chip. The BIOS will use the 8-leds for a startup animation, then use them as a progress bar for POST, then the last 2 leds flashes during Int13h Read/Write operations. The 4 digits 7-segment display is used also for displaying each step, error codes (and error information, like address and memory bank). The board has two analog inputs for knobs (potentiometers); these are connected to a differential op-amp setup; when the resistance changes they will trigger an interrupt and the channel identifier will be latched; the BIOS code handles these interrupts, doing I/O to the board for finding out the knob identifier + digitalized value; and at the end, it displays these as "A or B" plus a value from 0 to 100. One of these knobs was used for adjusting the Adlib output level.     

**The CGA Display board**

  The CGA video card was done turning the IBM schematics into logic expressions, then this logic was written into a few Atmel 16v8B GALs. The DRAM was replaced with SRAM. I used WinCUPL to compile the files and used my own-programmer that can be found also on my github. A RGB-component output port was added for better video signal; the composite signal wasn't well generated with the ATF16v8B's flipflops (the signal has noisy). Other parts differ, like the removing of lightpen option, driving the IOREADY signal using a transistor, cursor blinking using a 555 timer, etc. 

**The Adlib Sound board**

  The schematics were extracted following the Adlib component side/solder side photos from eBay (i found 2 sets of photos from 1989 and 1990 that differed). 

**The Memory board**

 This board holds the DRAM chips, having a RAS/CAS + databus interface.
  
**The Expansion Slots board**
 
 This boards holds the expansion slots and has a ISA-8 bit interface. Basically an ISA raiser.


**References**.
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
  * Ben Eater's EEPROM arduino programmer
  * Dr Dobbs journal delta modulation
  * Every datasheet you can think of.

**Thanks to people i actually e-mailed them...**
  - CFG register will set the GALs scheme? Manfred Winterhoff, the author of GALBlast project
  - Can HTC lag on XT-IDE? Charles Guzis, the author of XT-IDE, XT2PS2 project
  - Whats the use of this diode on Adlib? the author of an Adlib, Sergey Malinov 
