@echo off

echo .
echo .
echo .
echo A trocar as linhas da ROM original...
rem cga1 cga-char-rom\IBM_CGA_MDA.BIN  rom.bin
echo .
echo .
echo .

echo .
echo .
echo .
echo Gravar a PROM...
echo .
echo .
echo .
c:\development\tools\prom w -f=cga-char-rom\rom.bin -com=5