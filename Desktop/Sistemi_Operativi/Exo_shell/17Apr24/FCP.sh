#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore nel numero di parametri.Ci vuole almeno 3 parametri!
        exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto. Possiamo proseguire;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO DATO SIA UN NOME RELATIVO SEMPLICE
case $1 in 
*/*) echo Errore: $1 non in forma relativa semplice
exit 2;;
*) ;;
esac
X=$1 #chiamiamo X il primo parametro $1 che deve essere un nome relatico semplice

shift #facciamo un shift per andare a lavorare sugli altri N parametri

#CONTROLLO CHE GLI ALTRI N PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY

for i
do 
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

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

> /tmp/nomiAssoluti #creaiamo/azzeriamo il file temporaneo

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G  /tmp/nomiAssoluti #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

NRO=`wc -l < /tmp/nomiAssoluti` #calcoliamo il numero di file trovati che soddisfano la specifica
if test $NRO -eq 0
then 
    echo Errore. Non deve essere 0
    exit 5
else 
    echo il numero totale di file che soddisfano la specifica è $NRO 

    #va creata una nuova directory con nome relativo semplice X
    mkdir $X 
    cd $X #ci spostiamo dentro la nuova diretory trovata 
    for F in `cat /tmp/nomiAssoluti`
    do 
        #creiamo un link hardware che abbia lo stesso nome relativo semplice del file trovato
        ln $F $X 
    done 
fi


rm /tmp/nomiAssoluti #cancelliamo il file temporaneo
