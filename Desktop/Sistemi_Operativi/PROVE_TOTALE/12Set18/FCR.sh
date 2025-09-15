#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #variabile per contare il numero di file trovati
files= #per stockare i file trovati

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
         #SCRIVERE IL CODICE DELLA SPECIFICA
         #incrementiamo count
         count=`expr $count + 1`
         files="$files $F" #mettiamo dentro la nostra lista il file trovato
    fi 
done 

if test $count -ge $2 -a $count -le $3 #se il numero di file è compreso fra H e K
then 
    echo DIRECTORY TROVATA `pwd`
    #invochiamo la parte C
    echo INVOCAZIONE DELLA PARTE C CON PARAMETRI I NOMI DEI FILE TROVATI che sono $files 
    #main $files 
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 