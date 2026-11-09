@echo on

set VELOCIRAPTOR_PATH="c:\development\velociraptor"
set TARGET_PATH="c:\development\dosbox\asm"
set SRC_PATH="E:\Hardware\ibm5160-XT\rom_test\rom_test"


del  %TARGET_PATH%\*.* /Q

echo ***** Copiar ficheiros
copy *.asm %TARGET_PATH%
copy *.inc %TARGET_PATH%
copy %SRC_PATH%\tty.inc %TARGET_PATH%
copy %SRC_PATH%\macros.inc %TARGET_PATH%
copy %SRC_PATH%\b_bda.inc %TARGET_PATH%

echo ***** Compilar no DOSBOX
"C:\Program Files (x86)\DOSBox-0.74-3\dosbox" -conf dosbox.config -noconsole

echo ***** Cortar o cabeçalho MZ
stamp copy %TARGET_PATH%\boot.exe boot.ima -s=512 -init=hd
