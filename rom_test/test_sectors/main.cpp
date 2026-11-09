#include "vl_conf.hpp"
#include "vl_types.hpp"
#include <string>
#include <iostream>
#include <fstream>
#define VL_ARRAY_INCLUDES_STR2NUM_CVT
#include "vl_array.hpp"
#include "vl_util.hpp"

#define TOTAL_C 0x792
#define TOTAL_H 0x10
#define TOTAL_S 0x3F 

W32 chsToLBA(W32 c_, W32 h_, W32 s_, W32 tC_, W32 tH_, W32 tS_)
{
  return (c_*tH_ + h_)*tS_ + (s_-1);
}
W64 maxLBA=0;
W64 errorExists=0;
std::vector<W32> diffs;
SW8 b[512];
W16 *b16 = (W16*)b;
W0 proc(FILE* fc_=0)
{
	W32 line = 1;	
	W64 c,h,s,seg,off,res;
	FILE *f = fopen("sectors.txt","rb");

	while(!feof(f))
	{
		fread(b, 1, 25, f);
		if (feof(f)) break;
		b[2]=0;
		b[5]=0;
		b[8]=0;
		b[13]=0;
		b[18]=0;
		b[23]=0;
		if (b[0]!='-')
		{
			if (fc_) printf("\n--");
			parseHex(b,   &c);
			parseHex(b+3, &h);
			parseHex(b+6, &s);
			parseHex(b+9, &seg);
			parseHex(b+14,&off);
		}else
		{
			s++;
			off+=512;
		}
		parseHex(b+19,&res);
		W32 LBA = chsToLBA(c,h,s,TOTAL_C, TOTAL_H, TOTAL_S);
		if (fc_)
		{
			printf("\n L=%04u LBA=%06Xh %02X,%02X,%02X;%04X:%04X;%04X",
				line, LBA,c,h,s,seg,off,res);

			
			fseek(fc_, LBA*512, SEEK_SET);
			fread(b16, 512, 1, fc_);
			W16 res2 = 0;
			for (W32 i=0; i< 256; i++)
			{
				res2 = res2 ^ b16[i];
			}
			bool diff = res!=res2;
			printf(" x: %04X %s", res2, diff?"ERR":"OK");
			if (diff)
				diffs.push_back(line);
			errorExists|=diff?1:0;
		}
		else
		{
			maxLBA = VL_MAX(LBA, maxLBA);
			//printf("max. LBA: %Xh" ,maxLBA);
		}
		line++;
	}
	fclose(f);

}
W32 main()
{
	proc();
	std::string cmdstr = "disky r -d=PhysicalDrive3 -f=\"dump\" -S="+vl::util::t2str(maxLBA+1);
	::system(cmdstr.c_str());
	FILE *f = fopen("dump","rb");
	proc(f);

	if (diffs.size())
	{
		printf("\n NAO COINCIDE! \n");
		for (W32 i=0; i<diffs.size(); i++) printf("\n  linha %i", diffs[i]);
	}
	else	
		printf("\n OKKKK!!!!");

	printf("\n ; Tabela LBA > xor");
	for (W32 j=0; j<= maxLBA; j++)
	{
		fseek(f, j*512, SEEK_SET);
		fread(b16, 512, 1, f);
		W16 res2 = 0;
		for (W32 i=0; i< 256; i++)
			res2 = res2 ^ b16[i];
		printf("\ndw 0%04Xh ; LBA:%04X",res2,j);
	}


	fclose(f);
	VL_PAUSE
}