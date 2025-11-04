#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore sul numero di parametri perché ci vuole almeno 3 parametri.
        exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto e ora in poi proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN SINGOLO CARATTERE
case $1 in
?) ;;
*)      echo Errore: $1 non singolo carattere
        exit 2;;
esac
c=$1 #salviamo il valore di $1 dentro c

shift #facciamo un shift per andare a lavorare sugli altri parametri

for i 
do 
    #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 3
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 4;;
        esac 
done 

#controllo sul numero e tipo di parametri FINITO 

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

#INVOCAZIONE RICORSIVA
for D 
do 
    echo fase per $D
    FCR.sh $D $c #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!