#!/bin/sh
#file ricorsivo FCR.sh

cd $1 #ci spostiamo nella directory giusta

count=0 #per contare il numeri di file che soddisfano la specifica
LB= #serve per calcolare la lunghezza in byte


for F in *
do 
    if test -f $F -a -r $F -a -w $F #se il file esiste ed è leggibile e scrivibile allora
    then 
        #calcoliamo la sua lunghezza in byte
        LB=`wc -c < $F`
        #calcoliamo il numero di occorenze del carattere Cx

        if test $LB -eq $2 
        then 
            if test grep $3 $F 
            then 
                count=`expr $count + 1` #se soddisfa specifica allora incrementiamo il numero di file trovati
            echo `pwd`/$F >> $4 #mettima il file trovato dentro la nostra variabile temporanea
            fi
        fi 
    fi 
done 

if test $count -ge 1 #se abbiamo trovato almeno un file
then 
    echo directory trovata `pwd`
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 $4 #chiamata ricorsiva
    fi 
done 







