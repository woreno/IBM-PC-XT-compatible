@ECHO ON
@echo este e o make da BIOS:
@echo    - separa em planos cga e comprime a imagem em ../images/in.bmp
@echo    - comprime o som em ../sounds/test.wav
@echo    - junta os dois alinhados a 16bytes
@echo    - guarda na rom 
@echo    - cria um ficheiro de offsets (na verdade em segmentos)
@echo    - guarda no inicio da rom

@rem **********************************************************
@rem OPTIONS / OPÇOES
set     PROM_COM_PORT=5
set     WRITE_PROMS=1
@rem **********************************************************
@rem PATH / CAMINHOS
set     DEVTOOLS_PATH=c:\development\tools
set VELOCIRAPTOR_PATH=c:\development\velociraptor
set       DOSBOX_PATH="c:\Program Files (x86)\DOSBox-0.74-3"
set       TARGET_PATH=c:\development\dosbox\asm
set       UPDATE_PATH=UPDATE
set      IBM_ROM_PATH=E:\docs\arquiteturas\PC XT\rom-IBM5160-XT
@rem **********************************************************

@rem ADRESSING / ENDERECAMENTO
set      RESOURCE_SEG=0xF000
set      RESOURCE_OFF=0x0
set          JUMP_SEG=0x7FF
set          JUMP_OFF=0x7FF0
set          CHAR_OFF=0x7A6E
set      ABS_ROM1BASE=0xF800
set  ABS_RESOURCE_SEG=0xF000
set      ABS_JUMP_SEG=0xFFFF

echo .
echo ***** Apagar ficheiros antigos...
echo .

del  %TARGET_PATH%\*.* /Q
del *.pkg      /Q
del *.bin      /Q
del resources  /Q
del jump       /Q
del pkgmap     /Q
del pkgupd     /Q
  
echo .
echo ***** COPYING FILES TO %TARGET_PATH%
copy %VELOCIRAPTOR_PATH%\include\vl_conf.hpp %TARGET_PATH%
copy %VELOCIRAPTOR_PATH%\include\vl_util.hpp %TARGET_PATH%
copy %VELOCIRAPTOR_PATH%\include\vl_types.hpp %TARGET_PATH%
copy *.hpp %TARGET_PATH%
copy *.cpp %TARGET_PATH%
copy *.asm %TARGET_PATH%
copy *.inc %TARGET_PATH%
copy bios\*.* %TARGET_PATH%
copy cpp\*.* %TARGET_PATH%
copy draw\*.* %TARGET_PATH%
copy init\*.* %TARGET_PATH%
copy init\res\*.* %TARGET_PATH%
copy menu\*.* %TARGET_PATH%
copy res\*.* %TARGET_PATH%
copy text\*.* %TARGET_PATH%
copy spk\*.* %TARGET_PATH%
copy fat\*.* %TARGET_PATH%

echo .
echo ***** COMPILING IN DOSBOX
%DOSBOX_PATH%\dosbox -conf dosbox.config -noconsole
IF %ERRORLEVEL% NEQ 0 GOTO process_error 


echo .
echo ***** COMPILE RESULTS
type %TARGET_PATH%\BIOSERR.TXT

@echo .
@echo ---------------------------------------------------------
@echo ***** CUTTING BITMAPS INTO CGA PLANES (COL 166)
%DEVTOOLS_PATH%\bmp2cga s images/in.bmp images/out1.cga images/out2.cga -impal=166

echo .
echo ---------------------------------------------------------
echo ***** COMPACTING WITH RLE EACH CGA PLANE
%DEVTOOLS_PATH%\rle c images/out1.cga images/out1.rle
%DEVTOOLS_PATH%\rle c images/out2.cga images/out2.rle

@echo .
@echo ---------------------------------------------------------
@echo ***** COMPACTING WAVE SOUND with DELTAMOD
%DEVTOOLS_PATH%\acp c -fin=sounds/test.wav -fout=sounds/test.acp -mbits=2 -var
%DEVTOOLS_PATH%\acp d -fin=sounds/test.acp -fout=sounds/testd.wav

@echo .
@echo ---------------------------------------------------------
@echo ***** RESOURCES: JOINING "CGA PLANES" + "SOUND" em ALIGNED RESOURCES 
@echo                   a 16bytes, e emitindo um ficheiro de offsets
%DEVTOOLS_PATH%\glue resources images/out1.rle images/out2.rle sounds/test.acp -a=16 -f=resmap.bin  -fadd=%RESOURCE_SEG% -ftagsz=2 -fdiv=16
	
@echo .
@echo ---------------------------------------------------------
@echo ***** BIOS: STRIPPING MZ HEADER, STAMP DATE/HOUR 
@echo *****       STAMP RESOURCE ABSOLUTE SEGMENTS 
@echo *****       e xsum
%DEVTOOLS_PATH%\stamp copy %TARGET_PATH%\bios.exe bios.bin -s=512
%DEVTOOLS_PATH%\stamp size       bios.bin            -d=2 -tagsz=2
%DEVTOOLS_PATH%\stamp datetime   bios.bin            -d=4 -tagsz=1
%DEVTOOLS_PATH%\stamp copy       resmap.bin bios.bin -d=16
%DEVTOOLS_PATH%\stamp xor        bios.bin            -d=0 -tagsz=2

@echo .
@echo --------------------------------------------------------
@echo ***** JUMP: CREATE OPCODES FOR JUMPING TO SECOND BIOS (F8000) 
@echo *****       AT STARTUP 
%DEVTOOLS_PATH%\stamp new jump 16
%DEVTOOLS_PATH%\stamp str jump -words=0xEA,0x30,0x00,0x00,0xF8,'m','o','r','e','n','o'  

@echo .
@echo ---------------------------------------------------------
@echo *** CREATE BIOS PACKETS, RESOURCES PACKETS AND JUMP
@echo *** MARKING TO BE INSTALLED AND SIGNED 
	rem offset
	rem  0 tipo (1 byte)
	rem  1 segmento

	%DEVTOOLS_PATH%\stamp copy bios.bin bios.pkg -d=10
	%DEVTOOLS_PATH%\stamp str  bios.pkg -d=0 -str=D
	%DEVTOOLS_PATH%\stamp str  bios.pkg -d=1 -tagsz=2 -words=0
	%DEVTOOLS_PATH%\stamp str  bios.pkg -d=3 -tagsz=2 -words=%ABS_ROM1BASE%
	%DEVTOOLS_PATH%\stamp size bios.pkg -d=5 -tagsz=4
	%DEVTOOLS_PATH%\stamp xor  bios.pkg -d=9 -tagsz=1 

	%DEVTOOLS_PATH%\stamp copy resources res.pkg -d=10
	%DEVTOOLS_PATH%\stamp str  res.pkg -d=0 -str=D
	%DEVTOOLS_PATH%\stamp str  res.pkg -d=1 -tagsz=2 -words=0
	%DEVTOOLS_PATH%\stamp str  res.pkg -d=3 -tagsz=2 -words=%ABS_RESOURCE_SEG%
	%DEVTOOLS_PATH%\stamp size res.pkg -d=5 -tagsz=4
	%DEVTOOLS_PATH%\stamp xor  res.pkg -d=9 -tagsz=1 
	
	%DEVTOOLS_PATH%\stamp copy xtchar.pkg -d=10
	%DEVTOOLS_PATH%\stamp str  xtchar.pkg -d=0 -str=D
	%DEVTOOLS_PATH%\stamp str  xtchar.pkg -d=1 -tagsz=2 -words=0
	%DEVTOOLS_PATH%\stamp str  xtchar.pkg -d=3 -tagsz=2 -words=%ABS_ROM1BASE%
	%DEVTOOLS_PATH%\stamp size xtchar.pkg -d=5 -tagsz=4
	%DEVTOOLS_PATH%\stamp xor  xtchar.pkg -d=9 -tagsz=1 

	%DEVTOOLS_PATH%\stamp copy jump jump.pkg -d=10
	%DEVTOOLS_PATH%\stamp str  jump.pkg -d=0 -str=D
	%DEVTOOLS_PATH%\stamp str  jump.pkg -d=1 -tagsz=2 -words=%CHAR_OFF%
	%DEVTOOLS_PATH%\stamp str  jump.pkg -d=3 -tagsz=2 -words=%ABS_JUMP_SEG%
	%DEVTOOLS_PATH%\stamp size jump.pkg -d=5 -tagsz=4
	%DEVTOOLS_PATH%\stamp xor  jump.pkg -d=9 -tagsz=1
	
	%DEVTOOLS_PATH%\stamp new  end.pkg 1
	%DEVTOOLS_PATH%\stamp str  end.pkg -d=0 -str=E 
	
	echo .
	echo ---------------------------------------------------------
	echo *** CREATE UPDATE PACKET WITH THE PREV LAST PACKETS GLUED 
	echo *** em uma unidade ou enviado por porta serie
	%DEVTOOLS_PATH%\glue pkgupd bios.pkg  res.pkg xtchar.pkg jump.pkg end.pkg -f=pkgmap -freserved=15 -fnum=1 -ftotal=4
	%DEVTOOLS_PATH%\glue update.pkg pkgmap pkgupd

	%DEVTOOLS_PATH%\stamp str      update.pkg -d=0  -str="xtupdate"
	%DEVTOOLS_PATH%\stamp datetime update.pkg -d=8  -tagsz=1
	%DEVTOOLS_PATH%\stamp xor      update.pkg -d=14 -tagsz=1 

 @echo .
 @echo *** -------------------------------------------------
 @echo *** SAVE THE PACKET INTO 2 DISKS
 @echo *** (to test the UPDATE)
 copy update.pkg %TARGET_PATH%\update.pkg
 copy update.pkg update\update.pkg 
 %DEVTOOLS_PATH%\fatty updateA.img -dir="update/" -f=fat12 -t=hd
 copy updateA.img updateB.img


 
IF %WRITE_PROMS% EQU 0 GOTO process_no_write_proms
 
@echo ---------------------------------------------------------
@echo ***** WRITE the SECOND BIOS ROM (PHYSICAL ADDRESS F8000)
@echo ***** DUE CPU jumps FOR THIS ADDRESS
%DEVTOOLS_PATH%\prom w -f=jump -a=%JUMP_OFF% -com=%PROM_COM_PORT%	
%DEVTOOLS_PATH%\prom w -f=bios.bin -com=%PROM_COM_PORT%
 
%DEVTOOLS_PATH%\stamp copy "%IBM_ROM_PATH%\F800.BIN"  xtchar.bin -s=%CHAR_OFF% -sz=0x400
%DEVTOOLS_PATH%\prom w -f=xtchar.bin -a=%CHAR_OFF% -com=%PROM_COM_PORT%

@echo ---------------------------------------------------------
@echo ***** WRITE THE FIRST BIOS (PHYSICAL ADDRESS F0000)
@echo ***** WITH THE RESOURCES
%DEVTOOLS_PATH%\prom w -f=resources -a=%RESOURCE_OFF% -com=%PROM_COM_PORT%
copy update.pkg F:\

goto process_ok
:process_error

@echo *
@echo *
@echo *
@echo ***********************************
@echo ********** SOME FAILURE **********
@echo ***********************************
@echo 1
goto process_end


:process_ok

@echo *
@echo *
@echo *
@echo *******************************
@echo ********** SUCCESS ************
@echo *******************************
goto process_end


:process_no_write_proms

@echo *
@echo *************************************
@echo *****   ROM NON WRITABLE          *** 
@echo *************************************


:process_end