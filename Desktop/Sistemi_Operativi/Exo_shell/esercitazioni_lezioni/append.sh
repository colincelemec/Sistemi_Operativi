#!/bin/sh

#controllo sul numero di parametri

case $# in 
1) echo il numero di parametri passati è $#
    #verfichiamo che sia un nome di file
    if test -f $1
    then 
        echo nome file: $1
        #il contenuto del standard output deve essere scritto in append sul file
        cat >> $1
    fi;;
2) echo il numero di parametri passati è 2 
        #verfichiamo che siano un nomi di file
        if test -f $1 -a -f $2
        then 
            echo primo file: $1
            echo secondo file: $2
            #il contenutodel file il cui nome è passato come primo parametro deve essere scritto in append sul file il cui nome è passato come secondo parametro
            cat < $1 >> $2
        fi;;
esac


