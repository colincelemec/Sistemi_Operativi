#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #per contare i file che soddisfano la specifica
LN= #per calcolare il numero di linee del file che contengono il carattere Cz

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
         LN=`grep $2 $F  | wc -l`
         if test $LN -ge 2 #se abbiamo trovato almeno 2 linee
         then 
            count=`expr $count + 1` #incrementiamo count
            echo `pwd`/$F >> $3 #inseriamo il file trovato dentro il nostro file temporaneo
        fi 
    fi 

done 

if test $count -ge 1 #se abbiamo trovato almeno 1 file
then 
    echo DIRECTORY TROVATA `pwd`
fi 

for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 