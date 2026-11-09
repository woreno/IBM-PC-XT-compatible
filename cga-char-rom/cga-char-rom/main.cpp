#include "vl_conf.hpp"
#include "vl_types.hpp"
#include "vl_util.hpp"
#include "vl_con.hpp"
#include <conio.h>

// este é 
// estas sao as labels no diagrama (dst)
W32 ra=0;
W32 cc=3;
W32 cgasel=11;
W32 src_addrmap[]=
{ //src..dst
   0, // A0
   1, // A1
   2, // A2
   4, // A3
   6, // A4
   9, // A5
   10, // A6
   3, // A7
   7, // A8
   8, // A9
   11, // A10
   5, //A11
}; 

W32 src_datamap[]=
{ // dst // src
      0, // 0
      1, // 1
      7, // 2
      2, // 3
      6, // 4
      3, // 5
      5, // 6
      4  // 7
};
  

// ROM CGA/MDA
//	 13  12 11             3        0 
//		[     |    caractere | linha ]
//          \___ 00: mda (MSB da linha=0)
//          \___ 01: mda (MSB da linha=1)
//          \___ 10: cga
//          \___ 11: cga (segunda fonte)
//
//   * se MDA o bit 0..3 tem LSBs, e bit 11 tem o MSB da linha (14 linhas)
//   * se CGA o bit 12=1 e bit 11 seleciona a fonte   
SW32 ROM_A11 = 0;
SW8  *buffer; W8*dstBuffer;
#define ADDR_AT(CHAR_, LINE_)  ((ROM_A11<<11) | ((CHAR_)<<3) |((LINE_)&7) | ( ( (LINE_>>3)&1 )<<11 ) )
#define BUFFER_AT(CHAR_, LINE_) buffer[ ADDR_AT(CHAR_,LINE_) ]

SW32 fileSize;
W0   prnChar(SW32 atx_, SW32 aty_, W8 char_, SW32 lines_)
{
	VL_SCR_SETCURSOR((atx_), (aty_))
	printf("%u '%c'", char_, char_);
	aty_++;
	for (SW32 r=0; r< lines_; r++)
	{
		for (SW32 c=0; c<8; c++)
		{
			VL_SCR_SETCURSOR((atx_+c), (aty_+r))
			W8 pixel = (BUFFER_AT(char_,r)>>(7-c))&1;
			printf("%c", pixel?219:' ');	
		}
	}
}
SW32 main(SW32 nargs_, SW8 **argvec_)
{
	SW32 lines;
	nargs_--;
	argvec_++;

	if (!strcmp(argvec_[0],"cga2"))
	{
		ROM_A11   = 2;
		lines = 8;
	}
	if (!strcmp(argvec_[0],"cga1"))
	{
		ROM_A11   = 3;
		lines = 8;
	}
	else if (!strcmp(argvec_[0],"mda")) 
	{
		ROM_A11   = 0;
		lines = 14;
	}

	nargs_--;
	argvec_++;
	if (nargs_!=2) return 1;
	vl::util::loadFile(argvec_[0], 
						&buffer,
						&fileSize, 
						true);
	SW8 key=0;
	SW8 deltar=3, deltac=12, startChar = ' ';
	while(1)
	{
		VL_SCR_CLEAR
		for (SW32 r=0; r< deltar; r++)
			for (SW32 c=0; c< deltac; c++)
			{
				prnChar(10*c, r*(lines+1), startChar+(r*deltac+c), lines);
			}
		key = getch();
		if (key==27) break;
		startChar+=deltar*deltac;
	}

	SW32 dstSz     = 2*256*8;
	dstBuffer = new W8[dstSz];
	
	//for (W32 a=0; a<4096; a++)
	//{
	//	W32 dstAddr = vl::util::mapBitsWithSrcMap(src_addrmap, a, 12);
	//	W8 dstByte = vl::util::mapBitsWithSrcMap(src_datamap, buffer[a], 8);
	//	W8 dst2Byte = vl::util::mapBitsWithSrcMap(src_2nddatamap, dstByte, 8);
	//	dstBuffer[dstAddr] = dst2Byte;
	//}

	for (W32 f=0; f<2; f++)
	{
		ROM_A11   = f?3:2;
		for (W32 ch=0; ch<256; ch++)
			for (W32 l=0; l<8; l++)
			{
				W8  byte = BUFFER_AT(ch, l);
				W32 addr = ADDR_AT(ch, l) & 0xFFF;
				W32 dstAddr = vl::util::mapBitsWithSrcMap(src_addrmap, addr, 12);
				W8 dstByte = vl::util::mapBitsWithSrcMap(src_datamap, byte, 8);
				//printf("\n%X %X %X", addr, dstAddr, dstByte);
				//VL_PAUSE
				if (dstAddr >= dstSz)
				{
					printf("\n*** erro!");
				}
				dstBuffer[dstAddr] = dstByte;
			}
	}
	FILE *fd = fopen(argvec_[1], "wb");
	fwrite(dstBuffer, 1, dstSz, fd);
	fclose(fd);

	delete[] dstBuffer;
	return 0;
}



