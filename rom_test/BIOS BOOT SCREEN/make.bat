 
rem * isto é uma simulação do ecrã de boot da BIOS
rem * usando as mesmas funções. é criado um executável
rem * com um payload que são os planos CGA comprimidos

set          SRC_PATH="E:\Hardware\ibm5160-XT\rom_test\BIOS"
set VELOCIRAPTOR_PATH="c:\development\velociraptor"
set       TARGET_PATH="c:\development\dosbox\asm"

del  %TARGET_PATH%\*.* /Q

echo ***** Copiar ficheiros
copy %SRC_PATH%\bios\*.*		%TARGET_PATH%
copy %SRC_PATH%\cpp\*.*			%TARGET_PATH%
copy %SRC_PATH%\draw\*.*		%TARGET_PATH%
copy %SRC_PATH%\init\*.*		%TARGET_PATH%
copy %SRC_PATH%\init\res\*.*	%TARGET_PATH%
copy %SRC_PATH%\menu\*.*		%TARGET_PATH%
copy %SRC_PATH%\res\*.*			%TARGET_PATH%
copy %SRC_PATH%\text\*.*		%TARGET_PATH%
copy %SRC_PATH%\spk\*.*			%TARGET_PATH%
copy %SRC_PATH%\fat\*.*			%TARGET_PATH%
copy %SRC_PATH%\*.inc			%TARGET_PATH%
copy *.asm						%TARGET_PATH%
copy *.inc						%TARGET_PATH%

echo ***** Compilar no DOSBOX
dosbox -conf dosbox.config -noconsole

echo ***** Colar planos CGA + Som, e gerar um mapa de offsets 
echo ***** este é diferente do da ROM, aqui é alinhado, onde se poem
echo ***** o mapa estampado no inicio algures no inicio do codigo
echo ***** aqui se mapa e o conteudo se juntam (-fself) porque vao ser 
echo ***** lidos do mesmo ficheiro e alocados numa única zona 
glue resources ../BIOS/images/out1.rle  ../BIOS/images/out2.rle ../BIOS/sounds/test.acp -a=16 -f=resmap -ftagsz=2 -fdiv=16 -fa=16 -fself

echo ***** Juntar offsets recursos + recursos em "res.bin"
echo ***** nao fazer mais nada o programa vai alocar e ler tudo junto
glue %TARGET_PATH%\res.bin resmap resources

echo ***** Estampar alocacao de memoria no EXEC
echo ***** a reserva que o DOS faz para a imagem do EXEC
echo ***** fica exatamente restriginda a este,
echo ***** deixando-o alocar mais memoria pela INT21h
stamp size %TARGET_PATH%\main.exe	-d=0x4 -tagsz=2 -div=512
stamp size %TARGET_PATH%\main.exe   -d=0xA -tagsz=2 -div=16
stamp size %TARGET_PATH%\main.exe	-d=0xC -tagsz=2 -div=16

echo ***** copiar EXE para correr no virtualbox
del disk.img  /Q
fatty disk.img %SRC_PATH%\update.pkg  %TARGET_PATH%\main.exe -f=fat12 -t=hd 

echo ***** copiar EXE para SD
copy %TARGET_PATH%\main.exe F:

echo ***** Correr no DOSBOX
dosbox -conf dosbox.config2 -noconsole



