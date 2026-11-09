@echo off
setlocal enabledelayedexpansion
cls
echo * programacao das PALs para placa CGA *******
echo * ATF16V8B **********************************
echo *********************************************
echo * a detectar porta do programador ***********
rem set COMPORT=12
gal detect
set /A COMPORT=%ERRORLEVEL%
echo .
echo * porta detetada: %COMPORT% **************

goto :start

:repeat
	echo * preparar PAL para %1 **********
	pause
	echo on
	gal w -f=%1 -com=%COMPORT%
	@echo off
	echo .
	echo * codigo de saida: %ERRORLEVEL%
	if %ERRORLEVEL% == 1 (
		echo * falhou, a repetir 
		goto :repeat
	)
	echo * passando 'a proxima
	goto :eof

:start
for %%f in (*.jed) do call :repeat %%f
	
 
:eof