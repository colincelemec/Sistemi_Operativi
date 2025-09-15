#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore: Numero di parametri sbagliato. Ci vuole almeno 3 parametri
        exit 1;;
*)      echo DEBUG-Numero di parametri giusto e ora in poi possiamo proseguire!;;
esac

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NOME ASSOLUTO DI UNA DIRECTORY

case $1 in 
        /*) if test ! -d $1 -o ! -x $1 
            then
                echo Errore: $1 non directory o non attraversabile
                exit 2
            fi;;
        *)  echo Errore: $1 non nome assoluto
            exit 3;;
esac 

G=$1 #assegniamo a G il valore di $1
shift #facciamo un shift per andare a lovorare sugli N altri parametri

#GLI ATRI N PARAMETRI DEVONO ESSERE SINGOLI CARATTERI
for i 
do 
    #controllo che è un singolo carattere
    case $i in
    ?) ;;
    *) echo Errore: $i non singolo carattere
       exit 4;;
    esac
    C=$i #assegniamo a C il valore di $i
done 

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

#creiamo/azzeriamo il file temporaneo
>/tmp/nomiAssoluti

#INVOCAZIONE RICORSIVA
echo fase per $G
FCR.sh $G $C /tmp/nomiAssoluti

#Se siamo qui allora , la fase ricorsiva è finita!

#si deve ritornare su stdout il numero totale di file trovati

echo il numero totate di file trovati è: `wc -l < /tmp/nomiAssoluti`


for i in `cat /tmp/nomiAssoluti` #per ognuno dei file trovati
do 
    echo invochiamo la parte C passando come parametri F e i caratteri C1...CN
    #main $i $C
done 

rm /tmp/nomiAssoluti #cancelliamo il file temporaneo


