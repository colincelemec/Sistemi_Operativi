#!/bin/sh
#file ricorsivo FCR.sh

cd $1 #ci spostiamo nella directory giusta

LN= #variabile per calcolare la lunghezza in linee del file
count=0 #variabile per contare il numero di file che soddisfano la specifica

for F in * #per ogni elemento della directory corrente 
do 
    if test -f $F 
    then 
        #calcoliamo la lunghezza in linee de file
        LN=`wc -l < $F`
        if test $LN -gt $2 
        then 
            #incrementiamo la variabile del conteggio
            count=`expr $count + 1`
            #inseriamo (in append) il suo nome ASSOLUTO nel file temporameo, passato come terzo parametro
              echo `pwd`/$F >> $3
        fi 
    fi 
done


if test $count -ge 1 
then 
    echo directory trovata `pwd` 
    
fi 

echo il numero totale delle file trovate e che soddisfano la specifica è $count 

#invocazione ricorsiva 

for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 




