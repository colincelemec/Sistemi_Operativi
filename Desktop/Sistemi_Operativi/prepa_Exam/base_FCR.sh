#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta

count= #variabile per contare il numero di file trovati

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
         #SCRIVERE IL CODICE DELLA SPECIFICA
         #grep $i $F > /dev/null 2>&1 #per cercare un carattere dentro un file
    fi 
done 

#FARE EVENTUALI AZIONI CHIESTI DALLA TRACCIA

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 