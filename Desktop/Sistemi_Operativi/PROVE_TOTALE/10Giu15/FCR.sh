#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
NR= #serve per contare il numero di linee del file

count=0 #per il numero di file trovati

case $1 in 
*/$2)
    for i in *
    do
        if test -f $i  #se è un file
        then   
             NR=`expr wc -l < $F` #cancoliamo la lunghezza del file
             if test $NR -eq $3 #se il numero di righe è uguale a N
                then 
                    echo `pwd`/$F >> $4 #inseriamo nel nostro file temporaneo 
                    count=`expr $count + 1` #incrementiamo il numero di file trovati
             fi 
        fi 
    done;;
esac 

if test $count -ge 1
then 
    echo directory trovata `pwd`
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 $4#chiamata ricorsiva
    fi 
done 

