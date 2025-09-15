#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta

for F in * 
do 
    if test -f $F.$2 -a -r $F.$2 #se il file con estensione S1 esiste ed è leggibile 
    then 
        #mettiamo dentro la nostra prima variabile temporanea il file trovato
        echo abbiamo trovato il file $F con estensione $2
        echo `pwd`/$F >> $4
    fi 

    if test -f $F.$3 -a -r $F.$3 #se il file con estensione S1 esiste ed è leggibile 
    then 
        #mettiamo dentro la nostra  seconda variabile temporanea il file trovato
        echo abbiamo trovato il file $F con estensione $3
        echo `pwd`/$F >> $5
    fi 

done 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 $4 $5 #chiamata ricorsiva
    fi 
done 