#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta

L=0 #serve per calcolare la lunghezza in linee massima del file


for F in * 
do 
    if test -f $F.txt -a -r $F.txt #se il file esiste ed è leggibile 
    then  
        #calcoliamo la sua lunghezza in linee
        L=`wc -l < $F.txt`
        if test $L -lt $2 #se la lunghezza in linee del file trovato è strettamente minore di X
        then 
            echo `pwd`/$F.txt >> $3 #inseriamo nella nostra variabile temporaneo il file trovato
            
        fi
    fi 
done 

  

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 