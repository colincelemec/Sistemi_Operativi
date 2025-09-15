#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #per contare i file che soddisfano la specifica

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
         if grep $2 $F > /dev/null 2>&1 #se abbiamo almeno una occorenza del carattere C
         then 
            count=`expr $count + 1` #incrementiamo count
            echo abbiamo trovato il file $F che soddisfa la specifica
            echo `pwd`/$F >> $3 #mettiamo il file trovato dentro il nostro file temporaneo
        fi
    fi 
done

if test $count -ge 1
then 
    echo directory trovata `pwd`
fi 

for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 