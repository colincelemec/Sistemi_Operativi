#!/bin/sh

cd $1 #ci spostiamo nella directory giusta
F=0 #variabile per contare il numero di file trovati
D=0 #variabile per contare il numero di directory trovati

for i in *
do 
    if test -f $i #se è un file
    then
        echo [F]`pwd`/$i
        F=`expr $F + 1` #incrementiamo il numero di file trovati
    else
        if test -d $i #se invece è una directory
        then 
        echo [D]`pwd`/$i
        D=`expr $D + 1` #incrementiamo il numero di directory trovate
        fi
    fi
done

if test $F -eq 0 -a $D -eq 0
then 
    echo Attenzione la directory $1 è vuota
    exit 1
fi

echo Numero di file = $F 
echo Numero di directory = $D 