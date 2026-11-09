# marcações do fichiero de arranque
tronInit
clc
freq = 1*kilo;
offsets = ...
[0,...      #1
0.759,...   #2
1.763,...   #3
2.433,...   #4
3.327,...   #5
4.442,...   #6
5.552,...   #7
6.664,...   #8
7.605,...   #9
8.482,...   #10
9.462,...   #11
10.649,...  #12
11.937,...   #13
13.147,...   #14
14.656,...   #15
15.982,...   #16
17.808,...   #17 
19.494,...   #18
20.980,...   #19
22.465,...   #20 - i19
23.761,...   #30 - i20 
24.923,...   #40 - i21 
25.845,...   #50 - i22 
27.210,...   #é  - i23  
27.549,...   #sao - i24 
28.536,...   #meio-dia  - i25 
30.017,...   #meia-noite  - i26 
31.329,...   #e  - i27 
31.734,...   #horas  - i28 
32.847,...   #minutos  - i29 
34.360,...   #segundos  - i30 
35.832,...   #startup 
38.660,...   #
];


x=offsets.*freq


for i=1 : numel(x)
  x_ = x(i);
  s = 0x1000*floor(x_/(2^16));
  x_ = bitand(0xFFFF,x_);
  printf("\r\ndw 0%Xh,0%Xh",s,x_);
end

printf("\r\n"); 



x=([0:90]./360)*2*pi;

display("\r\nsinTab: ");
printf("\r\n db %u ", int8(sin(x)*120));
display("\r\ncosTab: ");
printf("\r\n db %u ", int8(cos(x)*120));





























