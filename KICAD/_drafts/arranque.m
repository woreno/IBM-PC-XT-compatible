clc
tronInit
R = 3.3*mega;
C = 420*nano;
t = 5*R*C
[' periodo de reset no arranque :',  toUnit(t,'s')]

 
dC=0x81;  # x=[[
dH=0xF;   # x=[[
dS=0x3F;  # x=[] <-inclusive

c1=0;
h1=1;
s1=1;
lba1=tronAskLBA(c1,h1,s1,dC,dH,dS);
[ ' c:' dec2hex(c1) ' h:' dec2hex(h1) ' s:'  dec2hex(s1)]
['lba: ' dec2hex(lba1)]

[c,h,s]=tronAskCHS(lba1,dC,dH,dS);
[' c:' dec2hex(c) ' h:' dec2hex(h) ' s:'  dec2hex(s) ]


%clocks
f = 14.31818*mega;
p = 1/f;

p_clk_88 = p + 2*p;
f_clk_88 = 1/p_clk_88;

p_pclk = 2* 3*p;
f_pclk = 1/p_pclk;

display(['OSC: '   toUnit(p,'s') ' ' toUnit(f,'Hz')]);
display(['CLK88: ' toUnit(p_clk_88,'s') ' ' toUnit(f_clk_88,'Hz')]);
display(['PCLK: '  toUnit(p_pclk,'s') ' ' toUnit(f_pclk,'Hz')]);

dma_refresh_sig_p = 72*p_clk_88;
dma_refresh_sig_f = 1/dma_refresh_sig_p;

display(['DMA REFRESH - PIT - 72CLOCKS: ' toUnit(dma_refresh_sig_p,'s') ' ' toUnit(dma_refresh_sig_f,'Hz')]);

ram_rows_per_s = (4*mili)/256;
ram_rows_per_f = 1/ram_rows_per_s;

display(['DRAM ROW REFRESH MAX: ' toUnit(ram_rows_per_s,'s') ' ' toUnit(ram_rows_per_f,'Hz')]);


bus_cycle_p =  p_clk_88 * 4;
bus_cycle_f =  1/bus_cycle_p; 

display(['NORMAL BUS ACCESS (Ex. RAM): ' toUnit(bus_cycle_p,'s') ' ' toUnit(bus_cycle_f,'Hz')]);





p = 10 * nano
toUnit(1/p,'Hz')
