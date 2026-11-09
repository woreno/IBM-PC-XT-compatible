// para testar com virtualbox é necessário criar
// uma porta e redirecionar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vl_conf.hpp"
#include "vl_types.hpp"
#define VL_ARRAY_INCLUDES_STR2NUM_CVT
#include "vl_array.hpp"
#include "vl_con.hpp"
#include "vl_cmdline.hpp"

SW8  programTitle[] = "Atualizacao ROM Manel\\PC XT";
Port port;
SW8 helloServerStr[] = "moreno-xtrom-server";
SW8 helloClientStr[] = "moreno-xtrom-client";
W0 txAck()
{
	port.tx(helloServerStr,strlen(helloServerStr));
}
W0 rxAck(W32 delay_=0)
{
	//printf("\nA' espera do cliente...");
	SW8 buff[512];buff[0]=0;
	while(1)
	{
		port.rx(buff, strlen(helloClientStr));
		if (!strncmp(buff, helloClientStr, strlen(helloClientStr)))
			break;
	}
	VL_SLEEP_MS(200+delay_)
}

SW8* filename = 0;

enum OPTION { OPTION_UPDATE, OPTION_DUMP };
OPTION option;
W64 comPort = 1;

W0 help()
{
	printf("\nServidor de imagens de BIOS ou DUMP NMI");
	printf("\nxtserver u [-com=X] -f=update.pkg........fornece atualizacao");
	printf("\nxtserver d [-com=X] -f=ficheiro-destino..faz o dump a memoria se NMI");
	printf("\n");
}
SW32 getArgs(W32 args, SW8** argVec)
{
	argVec++;
	args--;
	SW8 *arg = argVec[0];

	toUpperStr(arg);

	FAIL_IF(arg[1]!=0, "opcao desconhecida")
	if (arg[0]=='U') option = OPTION_UPDATE; else
	if (arg[0]=='D') option = OPTION_DUMP; else
	{
		FAIL("opcao desconhecida")
	}
	
	argVec++;
	args--;
	
	for (SW32 i=0; i< args; i++)
	{
		arg = argVec[i];
		toUpperStr(arg);
		if (!strncmp(arg,"-COM=",5))
		{
			FAIL_IF(parseUnsigned(arg+5, &comPort), "porta invalida")
			continue;
		}
		else
		{
			if (!strncmp(arg,"-F=",3))
			{
				filename = arg+2;
				continue;	
			}			
		}
		FAIL("argumento desconhecido")
	}
	FAIL_IF(filename==0, "falta especificar o ficheiro")	
	return 0;
}
SW32 update()
{
	FILE *f = fopen(filename, "rb");
	fseek(f, 0, SEEK_END);
	W32 fsize = ftell(f);
	W8 *buffer = new W8[fsize];
	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, fsize, f);
	
	rxAck(800); //-----1--------
	txAck(); //-----2--------
	printf("\nID correto.");
	 
	rxAck(800); //-----3--------
	W32 preambleSz = 20;
	port.tx(buffer, preambleSz); //-----4--------
	fsize-=preambleSz;
	W8* b = buffer+preambleSz;
	W32 toTx;
	rxAck(800); //-----5--------

	while(fsize)
	{
		toTx = VL_MIN(fsize, 512);
		//printf("\n to tx=%u, left=%u", toTx, fsize);
		port.tx(&toTx, 2);	// -----6--------	
		VL_SLEEP_MS(2)
		port.tx(b, toTx);	// -----7--------	
		fsize-=toTx;
		b+=toTx;
		rxAck();		// -----8--------	
	}	
	toTx=0;
	port.tx(&toTx, 2);	// -----6--------	
	return 0;
}
SW32 dump()
{
	W32 fsize = 512*1024; // + 16*1024;

	FILE *f = fopen(filename, "wb");
	W8 buff[512];
	printf("\n*** Dump\n");
	conProgressCallback(0, fsize);
	W32 fsize2=0, fremain = fsize;
	while(fremain)
	{
		// receber bloco	
		W32 toCopy = VL_MIN(fremain, 512);
		port.rx(buff, toCopy);	
		// receber xsum
		W8 xsum;
		port.rx(&xsum, 1);	
		// calcular xsum
		W8 cxsum=0;
		for (W32 i=0; i< toCopy; i++)
			cxsum^=buff[i];

		if(xsum!=cxsum)
		{
			printf("\n");
			for (W32 i=0; i< toCopy; i++)
				printf(" %02X", buff[i]);
			printf("\n <!> xsum: esperado %02X obtido %02X", cxsum, xsum);
			return 1;
		}
		fwrite(buff, 1, toCopy, f);
		fremain-=toCopy;
		fsize2+=toCopy;
		conProgressCallback(fsize2, fsize);

		port.tx(&cxsum, 1);
	}	
	conProgressCallback(fsize, fsize);
	fclose(f);
	return 0;
}

W32 main(W32 args, SW8** argVec)
{
	printf("\n%s", programTitle);
	
	SW8 portStr[]= "COM1";
	portStr[3]  = '0'+comPort;
	printf("\n*** Abrir porta COM%i...", comPort);
	FAIL_IF(port.open(portStr), "nao consigo abrir porta COM"); 
	port.setSerialParams(CBR_9600, 8,ONESTOPBIT,NOPARITY);	
	port.setTimeouts(10000,1,10000);

	printf("\n>");
	while(1)
	{
		W8 ch;
		port.rx(&ch, 1);
		printf("\n %02X %c", ch);
	}
	
	if(getArgs(args, argVec)) return 1;

	switch(option)
	{
		case OPTION_UPDATE: update(); break;
		case OPTION_DUMP: dump(); break;
	}

	printf("\n OK.\n");
	return 0;
}



