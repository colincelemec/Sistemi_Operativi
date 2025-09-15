#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
if test $# -ne 2
then 
    echo Errore sul numero di parametri. Ci vuole esattamente 2 parametri
    exit 1
fi 

echo DEBUG-OK: Numero di parametri giusto e ora in poi possiamo proseguire

#CONTROLLO SUL TIPO DI PARAMETRI

 #CONTROLLO CHE IL PRIMO PARAMETRO SIA NOME ASSOLUTO DI DIRECTORY
case $1 in 
/*) if test ! -d $1 -o ! -x $1 
    then
        echo Errore: $1 non directory o non attraversabile
        exit 2
    fi;;
*)      echo Errore: $1 non nome assoluto
        exit 3;;
esac 
G=$1 #assegniamo a G il valore di $1

#CONTROLLO CHE IL SECONDO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
#------------------------------------------------------------------#
case $2 in 
*[!0-9]*) echo Errore: $2 non numerico o non positivo
            exit 4;;
*)         if test $2 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 5
            fi;;
esac 
X=$2 #assegniamo a H il valore di $2
#--------------------------------------------------------------------#
    
#controllo sul numero e tipo di parametri finiti



#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

echo fase per $G
FCR.sh $G $X #Invocazione ricorsiva

#Se siamo qui allora , la fase ricorsiva è finita!