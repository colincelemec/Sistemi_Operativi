#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore sul numero di parametri. Ci vuole almeno 3!!
        exit 1;;
    *) echo DEBUG-OK: NUMERO DI PARAMETRI GIUSTO E POSSIAMO PROSEGUIRE;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 3
            fi;;
esac 
X=$1 #metto dentro X il valore di $1

shift #facciamo un shift per andare a lavorare sugli altri N parametri

#CONTROLLO CHE GLI N ALTRI  PARAMETRI SIANO NOMI DIRECTORY 
for G 
do 
case $G in 
        /*) if test ! -d $G -o ! -x $G 
            then
                echo Errore: $G non directory o non attraversabile
                exit 4
            fi;;
        *)  echo Errore: $G non nome assoluto
            exit 5;;
        esac 
done 

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G $X 0 #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!