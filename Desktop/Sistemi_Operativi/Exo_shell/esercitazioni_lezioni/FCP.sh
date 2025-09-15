#!/bin/sh

#controllo sul numero di parametri
if test $# -ne 1
then 
    echo Errore sul numero di parametri perché si vuole solo 1
    exit 1
fi

#controllo sul tipo del parametro che deve essere una directory con nome assoluto e trasversabile
case $1 in 
/*) if test ! -d $1 -o ! -x $1
    then 
        echo $1 directory non esistente o non trasversabile
        exit 2
    fi;;
*) echo $1 non nome assoluto
    exit 3;;
esac

#dato che il file comandi file-dir non è recursivo , non serve che settiamo la variabile PATH
