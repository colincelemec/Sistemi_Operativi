#!/bin/sh
#FCR.sh

cd $1 # ci spostiamo nella directory giusta

NG= #la variabile NG serve per il numero di linee trovare dal grep

for i in *
do 
    if test -f $i -a -r $i #se è un file ed è leggibile
    then 
        #calcoliamo quante linee terminano con il carettere t
        NG=`grep 't$' $i | wc -l`
        if test $NG -ge $2 #se il numero di linee calcolato è almeno X
        then 
            #inseriamo (in append) il suo nome ASSOLUTO nel file temporameo, passato come terzo parametro
            echo `pwd`/$i >> $3
        fi 
    fi 
done 

#invocazione ricorsiva 

for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 
    fi 
done 