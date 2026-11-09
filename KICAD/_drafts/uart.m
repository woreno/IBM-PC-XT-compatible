clc
# isto foi para confirmar porque o datasheet Async
# Comm adapter da IBM diz que a freq. do oscilador
# é de 18.432MHz (errado)
tronInit
xtal = 18.432*mega;


display(["frequencia base: " toUnit(xtal,'Hz')]);
for i=1:32
  f = xtal/i;
  bf = f/16;
  display([num2str(i) ' ' toUnit(f,'Hz') ' ' num2str(bf) ' bauds']);
end

display('');

xtal = 1.8432*mega;

display(["frequencia base (tipica do PC): " toUnit(xtal,'Hz')]);
for i=1:32
  f = xtal/i;
  bf = f/16;
  display([num2str(i) ' ' toUnit(f,'Hz') ' ' num2str(bf) ' bauds']);
end