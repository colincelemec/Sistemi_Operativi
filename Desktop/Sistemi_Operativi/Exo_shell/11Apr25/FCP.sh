#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2)  echo Errore sul numero di parametri. Perché ci vuole almeno 3 parametri
        exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto e ora in poi proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CODICE CHE SEPARA I PRIMI N PARAMETRI DAL L'ULTIMO
count=1 #la variabile count serve per capire quando abbiamo trovato l'ultimo parametro
params= #per accumulare i primi N parametri tranne l'ultimo
for i 
do 
    if test $count -ne $#
    then 
        #soliti controlli su nome assoluto e directory traversabile
        #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 2
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 3;;
        esac 
        params="$params $i" #inseriamo il parametro nella lista
    else
        #controllo sull'ultimo parametro
        #CONTROLLO CHE SIA UNA SEMPLICE STRINGA DI ALMENO DUE PARAMETRI
        #------------------------------------------------------------------#
        case $i in 
            ?) echo Errore : $i contiene un solo carattere, allora non è una stringa!!
                exit 4;;
            *) case $i in 
                */*) echo Errore: $i non in forma relativa semplice
                     exit 5;;
                  *) echo DEBUG: Ultimo paramatro $i OK, NON singolo carattere
                    ;;
                esac
        esac
        #---------------------------------------------------------------------#
        S=$i #mettiamo dentro S il valore di $i 
    fi 
        count=`expr $count + 1`
done 

#controllo sul numero e tipo di parametri FINITO

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/nomiAssoluti #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
for G in $params 
do 
    echo fase per $G
    FCR.sh $G $S /tmp/nomiAssoluti #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!
#calcoliamo il numero totale di file trovati globalmente
NRO=`wc -l < /tmp/nomiAssoluti`


if test $NRO -eq 0
then 
    echo Errore: Il numero di file trovati deve essere maggiore di 0
    rm /tmp/nomiAssoluti #cancelliamo il file temporaneo
    exit 6 
else 
    #riportiamo sullo stdout il numero totale di file trovati
    echo il numero totale di file trovati globalmente è $NRO 
    for F in `cat /tmp/nomiAssoluti`
    do 
        #va assegnato il bit di esecuzione per il proprietario file 
        chmod u+x $F 
    done 
fi 

rm /tmp/nomiAssoluti #cancelliamo il file temporaneo
