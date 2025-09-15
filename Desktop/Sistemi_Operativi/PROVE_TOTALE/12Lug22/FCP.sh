#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in
0|1|2|3) echo Errore nel numero di parametri. Ci vuole almeno 4 parametri
            exit 1;;
    *) echo DEBUG-OK: Numero di parametri giusto e possiamo proseguire;;
esac

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NOME ASSOLUTO DI UNA DIRECTORY

case $1 in 
/*) ;;
*) echo Errore: $1 non in forma assoluta
exit 2;; # valori diversi per maggiore usabilità!
esac
G=$1 #aessegniamo a G il valore di $1

#CONTROLLO CHE IL SECONDO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO

case $2 in 
*[!0-9]*) echo Errore: $2 non numerico o non positivo
            exit 3;;
*)         if test $2 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 4
            fi;;
esac 
L=$2 #assegniamo a L il valor di $2

#facciamo due shift per andare a verificare gli altri N parametri
shift
shift 

for i in $*
do 
    #CONTROLLO CHE IL  PARAMETRO SIA UN SINGOLO CARATTERE
    case $i in
    ?) ;;
    *)      echo Errore: $i non singolo carattere
        exit 5;;
    esac
done 

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/nomiAssoluti #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
 
echo fase per $G
FCR.sh $G $L /tmp/nomiAssoluti $*  #Invocazione ricorsiva

#Se siamo qui allora , la fase ricorsiva è finita!

echo il numero totale di file trovati è `wc -l < /tmp/nomiAssoluti`

for F in `cat /tmp/nomiAssoluti`
do 
    echo invochiamo la parte c
    #main $F $L $* 
done 

