#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
cont=0 #variabile che serve a contare il numero di occorenze del carattere Cx


for i in * 
do 
    if test -f $i -a -r $i 
    then 
         if  grep $2 $i #se contiene una occorenza del carattere x
         then 
            cont=`expr $cont + 1` #incrementiamo il numero di file trovati
            echo `pwd`/$i >> $3 #inseriamo dentro il nostro file temporaneo, il file trovato
        fi
    fi 
done 

if test $cont -ge 1
then 
    echo directory trovata `pwd`
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 