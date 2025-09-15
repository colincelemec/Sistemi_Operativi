#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
L=$2 #per il secondo parametro
TMP=$3
LN= #variabil per calcolare la lunghezza in linee
count=0 #per contare l'occorenza del carattere C

#facciamo di nuovo un triplo shift per eliminare i primi 3 parametri
shift 
shift 
shift 

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
         #calcoliamo la lunghezza in linee del file trovato
         LN=`wc -l < $F`
        if test $LN -eq $L #se la lunghezza in linee de file trovato è uguale a L
         then 
            #verifichiamo che contenga almeno una occorenza di tutti i caratteri
            for i 
            do 
                if grep $i $F > /dev/null 2>&1
                then 
                    count=`expr $count + 1` #incrementiamo count
                    #inseriamo il file trovato dentro il nostro file temporaneo
                    echo `pwd`/$F >> $TMP
                fi 
            done
        fi 
         
    fi 
done 

if test $count -ge 1 
then 
    echo directory trovata `pwd`
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 $* #chiamata ricorsiva
    fi 
done 