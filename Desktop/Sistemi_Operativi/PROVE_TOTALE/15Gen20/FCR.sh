#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
LC= #variabile per calcolare la lunghezza in caratteri del file trovato
cont=0 #per contare il numero di file che soddisfano la specifica

for F in * 
do 
    if test -f $F #se il file esiste 
    then 
         #SCRIVERE IL CODICE DELLA SPECIFICA
         LC=`wc -c < $F`
         if test $LC -eq $2
         then 
            cont=`expr $cont + 1` #incrementiamo count
            echo `pwd`/$F >> $3 #inseriamo nella nostra variabile temporanea il file trovato
        fi 

    fi 
done 

if test $cont -ge 1 #se abbiamo trovato almeno un file
then 
    echo DIRECTORY TROVATA `pwd`
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 