# IBM PC XT compatible (2022)
This is a IBM XT compatible built from scratch along 8 boards, 6 of them ISA 8-bit cards.

1.Design
2.Ram
3.Motherboard
4.Cable Continuity
5.Repairing the XT Bus
6.Making the IDE+RTC
7.Serial/Parallel/Joystick Port Adapter
8.Making the Ports Adapter
9.PS/2 to XT Keyboard translation
10.Drawers at Junk
11.Making of CGA and FD boards
12.Motherboard debug
13.Floppy Disk Adapter debug
14.CGA debug
15.IDE debug
16.Boot debug
17.DR-DOS
18.Adlib debug
19.Back to Motherboard debug
20.Ports debug
21.Moving on
22.Back to MS-DOS
23.References



1. Design

The design of the boards done here, is a bit personal. As other pcbs i do in-house, they are projected to be transfered with iron, drilled, then etched with iron perchlorate, using thought-hole chips, as many electronic and microcomputer hobbyists once did. To minimize errors on these manual procedures, circuits tracks and vias are made thicker. This project is about creating an IBM XT Compatible. This project began 5 years ago, when i started to documenting home computer systems, like older and newer PCs, another older systems like ZX Spectrum, MSX, Commodore, Amiga, Apple II, Macintosh, home and portable Sega, Nintendo and Sony entretainment systems, mainly for coding emulators. The IBM PC XT, was the second architecture to fully take notes after ZX Spectrum.
In 2021, after manually designing and building an Apple 2 compatible computer and an Adlib card, i ask my self "if it was possible to build electronically an IBM PC XT compatible" by the same means and tools (it seemed a big task by then). When i began, i wanted to be 8086 compatible, but the 16-bit bus and 16-bit memory organization logic, would make it impossible to fit in the maximum dimensions allowed by my own tools (A4 sheets), further the capacitance and thus distortion would be more pronounced on circuits. An after-end goal of this PC XT compatible project was to resume the M/OS, my simple multitask Operating System, totally coded in Assembler, but yet not with networking.
Most components on IBM PC XT are Intel, DRAM, ROM ICs. Some control, demuxes and muxes, buffers, latches, and other glue logic is done using 74 series.
2. Ram

The Dynamic RAM matrix was designed as a single board. It holds a matrix of 2 by 8 41256 and 2 by 8 4164 DRAMs. That is, the first 2 columns hold 512KB and the another 2 columns, holds 128KB, for a total of 640KB. For saving space, no additional parity logic and no parity DRAMs were added (similary as in another systems, as the Apple 2).
Each DRAM chip is structured like a grid, accessed by its row and column address know also as RAS and CAS address. DRAM chips are made besides another things, of capacitors that holds, each one, a charge related to its bit data, and these capacitors discharge over time; as these memories chips don't recharge their capacitors by themselves, they need external circuitry to do it. This operation of recharging is known as "refreshing", and can be done by reading each row of a DRAM chip. As those DRAMs needs refreshing, the IBM PC XT setups a dedicated DMA channel for reading the RAM, and a PIT channel as periodic signal to trigger the next DMA request. At each RAM access, the memory address is split in a signal to enable just a 8-chip column on the board, and RAS and CAS addresses with fast logic and delay lines. These last both addresses indexes a bit on the grid of a DRAM chip, so a enabled 8-chips column simultaneously indexes a full byte. DMA just need to rotate the lower 8-bits of the memory address. This is because this lower address is actually part of RAS address, and its rotation is enough to recharge all capacitors on 256 rows. Further the IBM PC XT circuits enables all the 4-chips columns during a DMA cycle to refresh a row but in all the board chips, during a DMA cycle. A pitfall is that the first 2 columns have 41256 DRAM chips. These ones have 512 rows instead of 256 rows; this means that 9-bit RAS address would be needed to refresh; however those chips are special, cause when refreshing their first half rows, both halves are refreshed, meaning all 512 rows are refreshed, so 8-bits are enough, for refreshing the both memory types aboard - the 41256 DRAM and 4164 DRAM chips. Another pitfall is that DMA is programmed as read-from-memory followed by write-on-I/O, so the DMA will read a random byte on one of those 256 rows, and write it on a random I/O address. This is a superfluous write will happen during a DMA cycle for refreshing the memory, so circuits on the PC, including boards on expansion slots, carefully ignore those random I/O writes if the DMA refresh line is active. Although refreshing has a periodic trigger, this one is subject to arbitration by same XT circuit. This circuit only will start the next DMA memory refresh if CPU is again free - so repeteated waitstates introduced by an I/O peripheral by changing its IORDY line, may disrupt memory coherence, if they are not brief. Circuits here are being tested for shorts.
3. Motherboard


This motherboard differs a bit from the original XT:
- It has built to be compatible, not strictly a clone.
- It has an Nec V20 CPU, instead of an Intel 8088; Nec is a better performant processor, fully compatible and less buggy than its original Intel counterpart.
- It has just 2 more modern ROMs, 1 not being used reserved for BASIC.
- It has a 2 Programmable Interruption Controllers; the second PIC interruptions pins are exposed just as pins, not being part of ISA-8 bit; they be used to support external wired interfaces like buttons.
- The overall layout is different, to serve the purpose of being more error prone to manual work; as tracks and vias are thicker RAM and expansion are built separated boards.
- In the original XT, IBM used a ROM to assist the memory organization; here this was done by mean of a few (fast) 74 XOR gates.
- The original XT also didn't allow lower ports to be decoded by boards, and here, the 7X range is allowed for supporting the Real Time Clock legacy IO port range, thus allowing RTC being installed on a board.
- theres a switch to set the PROM as writeable, so BIOS updates can be performed
4. Cable Continuity

Due to the wide and thicker tracks taking a lot of space, the boards have some discontinuity, mainly along power tracks. So some tracks must be connected with soldering wires.
If the circuits are factory-made, the design can hold a better power distribution, and also the boards can be filled with ground for reliability and safety. However, in old hand-made boards that are dense in tracks, those features are not frequently seen.
Just the mainboard track and hole count is quite large: 12330 tracks and 1373 drills; 150 tracks left to be connected manually, however this less than 1.2%...
5. XT Bus

The XT bus shown here was spoilt. Is being repaired and tested for shorts. After the photo paper transfer, this place was where broken tracks most likely to appear.
This is due that the tracks here are more thin and squished together than others in the board. Due to its thin size they are not easly corrected with pen markers. The red thing is varnish; the broken bus was corrected with small copper wires and solder - after proper drying the varnish helps to hold them a bit. The varnish is just a workaround, other products can be more appropriate for this job - as instance, plastic spray. What is important after applying them, is to wait to them dry properly.
6. IDE+RTC

The IDE+RTC and the display and ADC boards are being transfered.
When drilling use hand protection as possible. Here the drill is fixed to a hard base, so i was a bit relaxed.
This is the IDE + RTC board being drilled; IDE disk interface didn't exist at time of IBM PC XT.
The IDE is a 16-bit IO interface, but all XT boards fits an 8-bit ISA bus (earlier know as IBM IO Channel). However the IDE controller at the disk, only requires the full 16-bit data bus when is dealing with disk sectors transfers; other operations are performed using the lower 8-bit data bus.
For disk transfers then, 2 8-bit linear IO accesses are need to read or write a 16-bit value. For a sector transfer, a total of 512 bytes per sector, translates in 512 8-bit IO accesses on an IO data port.
The IDE board implements a two-way latching mechanism for the data IO port, so it can carry 16-bit transfers to, and from the IDE interface. If 2 successive write IO accesses are made to data port, at the first, 1 byte is latched; on the next access, the data byte is join with the last latched byte and delivered to the IDE interface along the signals needed for write. Similarly, on 2 successive read accesses on data IO port, the IDE interface higher 8-bits are latched and the lower 8-bits are delivered to the CPU, at the next access, the last 1 byte latched, is delivered to the CPU. So the IDE board, latches on demand, identifying 2 sucessive 8-bit data IO port, based on the last significant bit of the data port IO address.
When it writes, it latches the byte, and waits for second IO accesses to perform a 16-bit write through IDE interface. When it reads, performs a 16-bit read through the IDE interface, and delivers immediately the lowers 8-bits to the CPU, and in the next access, the other higher 8-bits latched previously.
Moreover, 16-bit code run on 8088 as in 8086, these CPUs are fully compatible. On the 8088, 16-bit IO or memory accesses is splitted automatically as 2 bus accesses by an internal unit, having the second 8-bit access, its address incremented by one. So the code to acomplish 16-bit transfers can also use 16-bit IO instructions.
The IDE latching mechanism is not my design. It was discovered on an old board by the members of Vintage Computer Federation. VCF run since then the fantastic XT-IDE project that integrates some optimizations and more features.
The IDE board also host a RTC (Real Time Clock) chip. It was a surprise that the RTC didn't provide a direct IO access. So some additional logic, as flipflop and other gates, were needed to complete IO accesses. RTC is a great addition to the computer, as it has a batery it can store data,
Motherboard IO integrated decodings circuits are different than original XT, to make an exception at lowers IO ports addresses, letting the range 7X free to be decoded by boards on expansion slots. This range includes the RTC legacy port address.
7. Back to 4-digits/8 leds/ADC

Back to the 4-digits, 8 leds and adc board, it's time to build the "user interface". This board is usefull for debugging the computer motherboard. Hence display steps, error codes, memory addresses, etc. So its mandatory to work properly as soon the system is first powered on. For that reason it was to be tested without recurring to the computer on the making.
The hand-made wooden button boxes and a dummy ISA card are used to manipulate directly the ISA bus signals, over a factory-made ISA backplane. The hand-made backplane could have also been used for this but wasn't ready yet. All hand-made boards are tested for shorts, and then power up, but not yet directly on the computer on the making.
This is so to not subject the PC hardware "under construction" to any possible losses, ensuring the boards safety before its use. After setting the address for IO decoding on the boxes, the 4-digits display, leds and ADCs are tested. Potentiometers are used to test the ADC chip, the digital conversion is reflected on the leds boxes, hence the leds light as binary number.
The board is tested again, but now in a real PC, a rusty Pentium MMX. This PC have a SDCard IDE adapter. So its fast, and Turbo C is called by autoexec.bat with a C code, to be less annoying, in case it fails.
The "User interface" is temporarily housed in a styrofoam plank connected by flat cables to the board. The C program tests 4-digits display, 8 leds, handles the interrupts when turning the knobs, and reads the ADC channels. A similar program for knob-triggered interrupts will be later be included in the ROM Program for the motherboard, for displaying Adlib and Speaker sound volume.
7. Serial/Parallel/Joystick Port Adapter

The port adapter features 3 ports:
- The serial port is offered by the UART 16550 chip - this is programmable for setting the communication proprieties (baud rate, parity, etc) amoung others settings. Because its IO interfaces are leveled at 5 volts, translation chips are needed - the MC14XX provides the proper voltage translation to -20V and 20 volts levels according the RS232 interface. This solution could be reduced further to just one chip MAX232 chip, a generation later. The connector is a DB9.
- The parallel port is made of buffers and latches. The output lines are latched. The input lines are just buffered. Some signals are Open Colector as they are bidirectional. Interrupts come directly from from the printer side.
- The Joystick port is made of 558 timers ICs, resistors and capacitors. Basically the joystick axes have two potentiometers that will change the charging time of the capacitors aboard. The 558 wiring is made so they can generate a output digital signal when the charge is above a certain threshold, and also to discharge fully the capacitors, by demand. The joystick buttons are just short-circuits over pull-ups on the board. The reset of caps, the axes signals, and the buttons offered as IO ports. The software samples the axes signal a few times, by discharging the capacitors, and counting the length of the signal, that is proportional to the position of the joystick axes. are proportional to the position of the joytsitck axes, and the status of the buttons.
The PC uses a DB15 pin connector, supporting 2 joysticks, which is easly mapped to Apple and Atari DB9.
8. PS/2 to XT Keyboard translation

A PS/2 to XT Keyboard translation was made in a perfboard. This is the same project AT2XT by Charles Guzis from Vintage Computer Federation. It uses the PIC12F629, and the software and hardware tools for PIC programming were home-made arduino-based.
This board would house the XT and PS/2 intefaces + PIC so making it on a cheap perfoard as faster.
The PIC is being programmed with a precompiled "Happy Birthday" song-player example. This was a easy way to test the PIC just with a speaker.
The PIC is playing the "Happy birthday". Everything seems OK, so it can programmed next for keyboard translation.
9. Drawers at Junk

At the junk i found 2 drawers. These may house all the computer. However i don't have enough skill for carpentry.
10. CGA+FD adapter making

This is the CGA video adapter.
This is the floppy disk adapter.
Did you notice? The printed FDD adapter looked a bit different from the previous CAD design. More on this later...
Starting the CGA video graphics tracks transfer to the copper board.
When studying the CGA video adapter i realized that i won't make with thicker tracks. The original CGA video adapter, was too big! Aside MCRTC (the Motorola CRT Controller) and VRAM (DRAM), it's all made of 74-series integrated circuits. In order to compact the design but mantain the things a bit old i bet for: ATF16V8, General Programmable Gate Arrays Logic and a quite modern Static RAM instead of Dynamic RAM.
I choose Static RAM here because i felt that "was enough" dealing twice with real DRAM on the IBM PC XT and Apple II motherboards (due to the refresh circuits hurdle). I choose the Programmable Gate Arrays because i recalled to being ancient (i used them at university back on 2000s).
PALs are fun their outputs are logic expressions of their inputs, so they can replace a lot of logic gates discrete chips and they are totally programmable. ATF16V8 used here are like PALs but more general, as their outputs can be programmed also as clocked flipflops. The software and hardware tools to program the ATF16V8 was also made inhouse using arduino. These chips are somewhat undocumented for programming without comercial tools. So i have to thanks the "GALBlast project page" because it was a lot of usefull information, also its author, Winterhoff for a doubt concerning the CFG register.
My first PC, bought in 1988, supported only CGA graphics or the MDA crisp text. It was an Amstrad Sinclair PC 200 with a fantastic S-12MM MDA monitor. This computer connected to a TV via composite, having CGA graphics, or to a MDA monitor with MDA graphics.
Please use hand protecting when using cutting tools. Those images were not to be aired.
Although my Amstrad Sinclair PC 200 had CGA and MDA, as the PC XT that came 7 years before, had its video circuits reduced to a couple of ICs, not two giant printed circuit boards! So it was a compact computer, technologic beautiful at that time.
Now about the immersion picture here: i reuse ferric chloride mixing the used one, with a bit of a new one. However resuing it can take a bit longer to develop. Gently rubbing with a sponge can accelerate the process.
Even after replacing much of the logic with GALs and SRAM, the card is still huge, but not as huge as the original CGA card!
I removed the IO access to the CRT controller ligh pen functionality, but added CGA to component signal conversion. This leads to way cleaner signals than the composite unique signal.
This is the Floppy Disk Adapter. It just carries a controller - i choose the Intel 82077AA. Its a full integrated chip from lates 80's to control floppy disk drives. As anything else, as material science, and manufacturing advanced, computer boards lose complexity due a greater integration. The IC used here belongs to a 4th generation or so, of floppy disk drives controllers.
Now at CGA component mounting.
I had the same PC until 1993 so i became to hate a bit CGA graphics. By those years, computers were getting better graphics like EGA or VGA, so games looked very impressive. The typical video mode for CGA had 320 by 200 pixels, 4 colors and 4 palettes, although 2 of them were just bright versions of the others 2. Besides black and white colors, the other choice were magenta and cyan, or red and green - a bit annoying.
So far the font ROM and SRAM, some latches and muxes are mounted. Some component placement finished now. Apart of GALs still to be developed, there is a font ROM, MCRT, SRAM option latches, data latches, and shift registers for video mixing and output.
Remaining power wiring is completed. The numbers on the board refers to the Gate Arrays yet to be socketed and programmed.
The 6 GALs were programmed and numbered. As the motherboard wasn't ready by then, CGA debugging was postponed...
11. Motherboard Debugging

After debugging and repairing a lot of tracks left behind, the cpu finally could jump to the ROM Program! The ROM Program is being stepped for debugging. This step do not uses the CPU Step Exception. The ROM Program instructions are followed by special WAIT instructions. At each of these, the CPU waits for its TEST PIN for manually going low. Shorting this pin is done here with a wire. So far the ROM Program just switches the leds lights, between even and odd positions.
At each iteration the ROM is being programmed with more code that tests more aspects of the system. Resuming and Reseting the CPU that were done before with wires, now are done by pushing buttons.
Borland Turbo Assembler (TASM) is used to compile the code. A script calls DOSBox to run TASM. After compilation and linking, inhouse-made software and hardware tools send it to arduino to program the ROM - this one an eletrically programmable ROM by Atmel 28C65_256.
As there's no RAM yet, a hack was done to keep the things simple: Some code uses calls but, returning to ROM addresses, instead of RAM. This way one cannot nest functions, but at least one can call them once, and keep things neat. Other small repetitive code is expanded by macros.
The PIT, the Programmable Interval Timer, and the PPI, the Programmable Peripheral Interface are now installed. After testing and repairing connections around these, the Speaker finally sounds. Moreover the speaker PIT channel frequency is changed at boot for test.
The jumper are under test; they are being reflected on the leds and on the 4-digits gourdgeous display. Those jumpers are connected to PPI. The Intel PPI was called like that because it offers a programmable way to define the functionality and layout schemes for their IO pins.
The DRAM board is now installed. DRAM needs to be refreshed. Later, a DMA Controller channel will be programmed to refresh the memory periodically using a PIT channel. For now, a read-after-write test is just performed. It is OK to test memory this way, without explicity refresh them with external circuits. In these DRAM chips a row needs to be refreshed at each 15 microseconds, while the minimal bus cycle is about 0.8 microseconds (4 CPU clocks) so there's enough time for both write and reads for testing purposes.
Here the read-after-write fails: The pattern AA wrote is read as EA, at the forth column. However this is just bus garbage due the DRAM access is still not working properly.
Here sometimes the PC halts the ROM Program at each reset. This was due an address line almost disconnected due to motherboard moves.
Without the DRAM board installed the ROM code is being improved to show the base segment under test and which chip on the column is failing (the data bit).
Here the ROM Program outer loop adds 800 to the segment register, while the inner loop iterates the 64KB offset. This was an error, as it should be adding 1000 to the segment register to test the next 64KB of DRAM.
Here the ROM code was updated to program the DMA controller to refresh the DRAM. To benchmark the memory hold time, a frequency generator as used as a trigger, instead of the PIT original signal. Is then easy to change the frequency of the refresh signal.
The ROM Program now programs the Programmable Interruption Controller. The values displayed are IRQ handlers addresses (segment and offset) that were copied to lower RAM. After checking these addresses are OK, interruptions are tested by pulldown the PIC pins.
Some DRAM chips had defect. 2 columns were desoldered and the computer is working now with just 512KB. Unfortunately, as the DRAM chips share tracks, if one chips fails can affect others. So displaying the column and bit failing, may not be always reliable to identify the affected DRAM chip.
The DRAM is tested in chunks of 64KB. Hence the segment register displayed adds with 1000. 4 leds display which column. There's only 2 columns of 256KB in use now. So far, 512KB DRAM was passed the write-read test. Due to defective ICs, the he last 256KB column had to be unsoldered.
12. FDC Debugging

This is the Floopy Disk Adapter first test. The ROM Program enabled the MOTOR ON signal on the first drive.
As stated previously this board had an error. An after-end goal of this PC XT compatible project was to resume the M/OS, my simple multitask Operating System, totally coded in Assembler. This one started 3 years before this project, but was not yet network ready. However i didn't take all the boards into account. I end designing the exapnsion with only 5 slots. To include a network card, 6 slots were need. So the floppy drive adapter has designed with an ISA slot. Fearing more capacitance on tracks with this extra slot, a transceiver was added between the FDC and the databus; This was a mistake, i only took into account explicit IO accesses, forgetting DMA cycles.
As there's was no easy way to make enable and direction signals taking into account both DMA and explicit IO cases for this transceiver, it had to be removed.
The ROM Code was updated to test track seeking. But this drive was faulty, seemed to make a short sound when seeking. So the floopy drive was its case removed. And yes, the floppy drive stepper was faulty.
PC floppy drives should be fully programmed with interrupts. In this test code however i ignore them waiting a bit before proceed to next instructions, later on, the code was redone take into account with interrupt handling, part of the disk service, INT 13. The first floppy drive was definitively dead.
This another floppy drive is working fine. Now is reading sectors linearly, displaying them along the tracks, on the gourdgeous 4-digit display. 2 leds displays which disk side. As there's no video card, is too soon to find out if is working well.
13. CGA Debugging

The CGA video card component mount is ready. IO decoding, clock division, option latches are being first checked. Something is not OK, the power-on sound is faster than the usual.
With these previous things working, the ROM Program setups the Motorola CRT Controller registers for setting one of each of the intended CGA videomodes. The controller output signals frequencies are then checked for correctness, that is how fast it rotates the local video addresses, and how fast are horizontal and vertical sync signals, etc.
As always, some things were forgotten to wire. It grows the mess at board's back. This is a video RAM read-after-write test. However isn't capable of detecting mirroring. Forgotten wiring are still checked and soldered. VRAM is likely not being latched and Vsync signals are affected.
This was the first composite signal output.
In this test, chars are written followed by a beep sound. Its position is also show on the 4-digit display. With this test one should able to quickly identify wiring problems along the VRAM, latches and shift registers.
Now some part of the written VRAM is finally displayed as text chars. It looks like has some mirroring, so higher addresses tracks may not be fully connected to the VRAM or they may even be shorted.
The ROM program now displays a matrix of 8 by 8 chars, 256 chars as total. Every char seems to be in place, the font ROM seems to be the problem. This font ROM was 2 fontfaces, which one is bold. A built in switch on the board, allows to switch between thin and bold fonts, and is being tested now.
For saving space around the font ROM chip, the address and data tracks were reordereed, and a program was made to reorder the font-bitmap accordantly, before writing it to the font ROM. This program had a mistake on its map definition.
The font ROM is now OK. The ROM Program displays the char matrix, looping the background and foreground text-attributes. Both are encoded on 16-bit words.
Shaky composite output means that color phase signal probably is being mixed to late with the remaining composite signal. I choose to go back to component signals and handle the composite case later, as this is a derived, but more complex signal.
The CGA first text mode 40 colums by 25 rows, seems to work, but theres a simple component color exchange.
After swapping the component wires the text front and background are correct.
To test the video graphic mode a bitmap was encoded at the ROM. Each byte holds 4 pixels. The displayed image seems wrong. A pixel-by-pixel test later, revealed that the 2-bit pixel order at each byte, was reversed. This was a wiring problem at the multiplexer, which accepts the shifted bit tuples, in case of the graphic modes.
This graphic mode seems OK.
More over the 4 palettes are being tested. The 40-column and 80-column text modes are being tested.
Almost all video modes were tested. Now the ROM Program that boots the machine can take advantage of this adapter to display a lot of information as BIOS screens do. The CGA video adapter is almost complete.
14. IDE Debugging

The testing of IDE adapter begins. A SD Card board is connected with a SDCard. The first 512bytes of info and status register are pooled.
The information buffer reads returns FF or 00 that's it - not working yet. Inspections of the SD Card first block buffers were used for identifying and repairing unsoldered pins and tracks at the board.
The read and write ROM code evolved to be part of INT 13 handling, the disk service. So now it should be able to boot a disk Operating System.
15. Boot Debugging

The DRAM grid was unsoldered to put DIP sockets. This was to make it easy to replace faulty ICs. Due that taken chips had to be cleaned from the remains of the solder mess, the DRAM board was keep to a minimum of 256KB - 1 column.
The ROM program is evolving to being more a PC BIOS. It tests memory, the timer interrupt request, scan some ports, scans the expansion for bootable ROMs, handles INT 10 for text writing, INT 13 for just IDE disk access, int 1 and 3 for debugging, others interrupts are dummy for now. Reads the first sector to RAM to boot from harddisk. By changing jumpers it can put the CPU to step instructions on boot and display CPU registers or display read buffers at INT 13.
By CGA design interferences are natural in 80column text video mode, but not so intense. CGA was a good effort to take advantage of MCRTC, with just 74 logic. However it was left to the software to avoid interferences when writing in this text-mode, waiting for the Vertical Blank Interval signal (VBI). Despite interrupt 10 is still a draft, uses this waiting. What i didn't know at the time was that VBI register pin was floating, so even chars were being displayed slower at this text video mode.
MS-DOS hangs at boot. I must knew what was wrong - all the software exceptions used by DOS to get information about the machine, and disk buffers are being scrutinized by single-step or breaking just at each INT.
The Single-Step or Trap flag is set at boot of after a software exception according the computer jumpers. At each instruction the CPU jumps to the exception address. The code for handling this was placed on lower RAM, on top of the BDA (the Bios Data Area). It just save registers, print them and restores them. In reality just a small critical code is on RAM, because BDA is too small and DOS can take ownership everythings above it. So to narrow things the handling happens in lower RAM, but resumes on the ROM.
512byte blocks are being read with INT 13, the 4-digit display show how many blocks remains on read-write operations lead by INT 13; leds are used to indicate which operation.
Tracking all instructions to the point of the hang is a tedious task. As keyboard interrupt handling was not being implemented so far the single-step optionally is resumed by the TEST pin button or automatically stepped according computer jumpers.
Where DOS is copying itself to memory and what is calling from the ROM program is being monitorized closely.
Although there's no automatically disassembly, the opcodes near the instruction pointer are also sometimes disassembled with a cheat-sheet. So far 3 hangs were detected and corrected. On boot, DOS needs the BIOS timer and machine type interrupts working properly. DOS also rewrites the trap exception address. So the ROM was expanded to service more calls, and the trap handler was coded to avoid the deadly move instruction.
After the DOS "starting message" and several calls to BIOS services, hangs or reboots were identified when trying to execute code at memory top. So i had to move further, suspecting about the IDE controller reading the disk and memory integrity.
The ROM program can now print a title to BIOS services under single-step according to computer jumpers.
The ROM program now has a checksum list, for each disk sector around DOS-related allocations created by a program. INT 13 now computes a 16-bit checksum over a sector and compares it against the expected value. A checksum error happen randomly sometimes, and its root cause as identified as the HTC chip that was used instead of LS. This was likely an effect of cumulative capacitance on the board wider circuits plus the less sharp signals that are expected from the HTC series. However DOS still hangs or reboots.
d=INT 13 caller address, X = checksum expected, L = LBA and ? = checksum computed
I decided to install Caldera DR-DOS (instead of MS-DOS) to track the booting process as comparasion.
16. DR-DOS

Caldera DR-DOS (previously Digital Research DOS) seems to not to crash or reboot. The booting process may be different than MS-DOS.
Do you know why in DOS typing a DIR command at root, can take more time than other directories when dealing with FAT12 or FAT16 volumes? - On these filesystems, the root directory have a fixed number of entry pages, then the OS was to loop all entries if not cached.
One of my favorite games back to my first PC was AlleyCat. This is a small game, with an executable image under 64KB. The code also doesn't perform any memory allocations, as it is a COM file. These COM apps run only within assigned code segment the same as their data segment. I was expecting that these kind of apps to run just fine - instructions or data memory accesses are repeatly being performed around this tiny memory space, so they are refreshing their own memory aside the periodic refreshes using DMA. So if theres is any problem related with memory allocations (as it seems, at the top), or with the periodic memory refreshes using DMA, those kind of apps are not expected to ill behave.
Other games loading comes next.
This is Digger another great small game.
INT 13 was expanded to support floppy disk drives; seek test is done at powerup.
Wonder if Adlib is working well.
17. Adlib

Each dual potentiometer is connected to the Adlib and Speaker to control the volume, and to the 4-digit display, leds and ADC adapter. When the potentiometer knob turns triggers an interrupt. The ROM Program handle this interrupt, reading the adapter ADC value and normalizing it, finally display it at 4-digit display plus the sound source (A or B).
The circuit designed is a bit flawed. The used dual-potentiometers are not linear, being a best approach to sound control. I want an analogic way to do it, so i choose an op-amp differential scheme. A downsize of this is that sometimes interrupts aren't generated at all if the knob is turning too fast. So when this happens, will not display the instantaneous volume sound value.
18. Back at motherboard debug

This is a deep memory test not shown before, also enabled by jumpers. In this test, after programming the DMA for refreshing, all the memory is filled with a pattern (designed to avoid cases mirroring), a few seconds countdown is started and then written patterns are check for errors. This test confirms that during the countdown period, the memory was consistently refreshed using DMA.
Sometimes the board isn't fitted properly on its slot. Memory seems to be OK.
This is buddokan, some lenghty games experience crashes.
This is nightmare on elm street, the graphics seems to be garbled.
Despite memory test are OK, games that request memory to DOS (Prince, Bubble Booble, etc) were likely to display artifacts, erratic movements or could quit unexpectly. Tiny Games as Alleycat, Digger, that are only loaded images under 64KB won't.
So games are being load again and again, to check if this there's an arbritation problem with DMA. The osciloscope is showing the DMA request signal for refresh, although periodic near 66.7KHz, has some phase offset on IO accesses, what is normal, because IO accesses introduces a mandatory extra 1 wait state. Although, no board here use the IOCHRDY line to induce more waitstates, my fear was the increased capacitance or even 74 chips, would make the circuits lag a bit, probably the enough to disrupt part of memory, due not being refreshed on time. Again, the frequency generator is being used as ensure the process.
This is an example of erratic movements as response to keys presses.
This is an example of graphics artifacts.
This is an example of an erratic loading.
I recalled that Sierra Games font characters were very similar to the ones in the CGA text video mode. Sierra for sure wasn't using the ROM services to write text in graphics mode, instead they directly used the bitmap memory address. To put things in context: the IBM PC XT rom has a copy of the font rom on their CGA adapter, because they need also to provide text writing in graphics mode, but this later rom was not addressable by the cpu.
Some apps would use the address of this copy instead of calling the video services for writing text. So it was my fault when drafting the video services handle to assume that i could encode the font-bitmap anywhere in the rom, as long i provided the text writing. This was solved moving the bitmap to the expected address.
The video service have some tricks - it allows to identify a char at a location in both text and graphic modes. However in graphic mode, as it doesn't have any access to the char previously written on screen, it will use pixel-by-pixel matching against the pixels and the font-bitmaps at a particular location for identifying the character.
19. Ports debug

Parallel and joystick ports were tested on the ISA backpane with the wooden boxes, cause their circuits are simple not needing any setup, accessible by IO decoding. However Serial port uses an chip know as UART (universal asynchronous receiver/transmitter) that needs programmatically to be set before communication.
OK, its working moving on.
20. Moving on...

Now returning to the previous point, ruled out memory consistency problems, seems there's a DOS memory allocation problem, causing games to exhibit erroneous behaviour. The Nightmare on Elm Street sometimes would throw explicit messages saying "pointer allocaiton null". However as in other games this was not a deterministic behavior. Moreover how could allocations fail randomly, as there's was no memory, if games ran from a fresh prompt.
Was DR-DOS giving more memory to processes than the available?
The ROM program was updated to have a BIOS-like portuguese menu activated by the ENTER key during boot. If the Real Time Clock Adapter is available, has battery and stores a magic cookie, the ROM will restore the stored user settings. Ports and interrupts definitions, Boot order, Updates from floppy, harddisk, or serial port among other features are offered. At boot screen and menu, is possible to start a deep memory test or test CGA, serial, parallel, joystick ports by hitting a key.
I decided to change some chips in the DRAM board and to install again MS-DOS.
21. Ending

For my surprise all the games were working. The most reasonable explanation that i can think of, is, as the chips are decades old, their performance might be affected over time, probably with the raise of temperature. This means that their hold time may get short due capacitor aging, or their speed may slower down due transistor aging.
I tested some games in X86box, an emulator with DR-DOS, reducing the available memory to 512KB and 384KB to confirm these behaviours. In fact, Nightmare On Elm Street will hang and throw a message "pointer allocation null", other games will check memory size before load and throw a warning to quit. So it wasn't any fault with DR-DOS.
Pictured here, there was still a bug with RTC date/time reading that was corrected later. The games run great. The BIOS menu has built in a hurry, so when cursors keys were pressed, all the screen was regenerated. The front case, features also 2 buttons, one for NMI Interruptions another for Reset. In IBM PC XT, NMIs are triggered on memory parity or ISA boards errors. NMI however, is maskable through an register. I took off this, so NMI was left to to debugging purposes. Many old games could avoid any attempt of debugging by disabling IRQs and NMIs, which could be used to inspect their code flow. This was a good way to insert an always-on debugger feature on this system.
I couldn't expand the memory to 640KB because, i bought some defective chips. This was the last footage of the Manel/PC working.
22. References

The Indispensable PC Hardware Book - Hans Peter Messmer; 1994
Digital Systems Project / Projecto de Sistemas Digitais (ISEL) - Victor Pimenta Rodrigues, Mário Seia de Araujo; 1988 (3th edition 2001)
IBM PC XT Technical Reference; 1983
Electrónica AC/DC/Analise circuitos - Santos, Guimarães e Oliveira, SA. Porto; 1980s
Interfacing to the IBM Personal Computer; Lewis C. Eggebrecht; 1983
Vintage Computer Federation (VCF)
GALBlast by Manfred Winterhoff
AT2XT Keyboard Adapter by Charles Guzis
XT-IDE by VCF members
HelpPC Reference Library by David Jurgens
Every datasheet you can think of.
People i actually e-mailed them because...
CFG register will set the GALs scheme? Manfred Winterhoff, the author of GALBlast project
Can HTC lag on XT-IDE? Charles Guzis, the author of XT-IDE, XT2PS2 project
