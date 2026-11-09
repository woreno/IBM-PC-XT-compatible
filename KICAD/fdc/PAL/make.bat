@echo off
echo * compilar antes com WINCUPL PAL0.PLD
echo   para ficheiro FDC_PAL0.jed 
echo   esta PAL e' para a placa FDC para validacao de
echo   endereco I/O e sentido do transceiver de 
echo   dados; se mudar a porta COM, usar -COM=X
echo   Inserir pal ATF no programador e tecla para continuar...
pause
gal w -f=FDC_PAL0.jed -com=3



