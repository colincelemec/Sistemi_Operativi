
x=`wc -l < p.txt`
echo x=$x

y=`wc -l < prova`
echo y=$y

z=`expr $x + $y`
echo la somma di $x e $y fanno $z


