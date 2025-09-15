#!/bin/sh
#FCR.sh

cd $1 #Ci spostiamo nella directory giusta

NR= #definiamo una variabile NR per contenere il numero di righe

trovato=false #definiamo unavariabile trovato che ci serve per sapere se abbiamo trovato almeno un file

for i in * #per ogni elemento della directory corrente
do 
    if test -f $i -a -r $i #se è un file ed è leggibile
    then 
        #calcoliamo il numero di linee
        NR=`wc -l < $i`
        if test $NR -eq $2 #se il numero di linee è quello richiesto
        then 
            #inseriamo (in append) il suo nome assoluto ne file temporaneo, passato come terzo parametro
            echo DEBUG-TROVATO FILE `pwd`/$i 
            echo `pwd`/$i >> $3 
            #e aggiorniamo la variabile trovato
            trovato=true
        fi 
    fi 
done 

#se abbiamo trovato almeno un file
if test $trovato = true
then 
    echo TROVATA directoty `pwd`
fi 

#solita invocazione ricorsiva

for i in *
do 
    if test -d $i -a -x $i 
    then 
        echo DEBUG-RICORSIONE
        FCR.sh `pwd`/$i $2 $3 