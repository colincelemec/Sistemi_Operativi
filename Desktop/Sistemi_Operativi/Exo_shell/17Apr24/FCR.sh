#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #per contare i file che si trovano all'interno della directory

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
        #dobbiamo verificare se il file si trova nella terza posizione all 'interno della directory
        count=`expr $count + 1` #incrementare count man mano che trovano un file nella directory
        if test $count -eq 3 #se arriviamo alla terza posizione del file nella directory
        then 
            #inseriamo il file trovato dentro il nostro file temporaneo
            echo abbiamo trovato un file in terza posizione
            echo `pwd`/$F >> $2 
        else #se non abbiamo trovato un file in terza posizione nella directory 
             #riportiamo su stdout il suo nome assoluto
             echo il nome assoluto del file trovato che non è in terza posizione è : `pwd`/$F 
        fi 
    fi 
done 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2  #chiamata ricorsiva
    fi 
done 