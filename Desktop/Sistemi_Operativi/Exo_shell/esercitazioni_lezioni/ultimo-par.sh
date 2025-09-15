#!/bin/sh

count=1
LIST= #variabile per memorizzare la lista dei parametri senza l'ultimo
LAST= #variabile per memorizzare l'ultimo parametro

if test $# -eq 0
then
    echo ERRORE: Ci vogliono parametri
    exit 1
fi


for i 
do 
    count=`expr $count + 1`
    if test $count -ne $#
    then 
        LIST="$LIST $i" #quindi memorizzano il paremetro corrente nella lista
    else
        LAST=$i
    fi 
done

echo Ultimo parametro = $LAST
echo Lista escluso ultimo = $LIST