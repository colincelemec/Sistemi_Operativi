#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO

case $# in 
0|1|2|3) echo Errore sul numero di parametri. Ci vuole almeno 4
            exit 1;;
    *)  echo DEBUG-OK: numero di parametri giusto. Ora in poi proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
#------------------------------------------------------------------#
case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 3
            fi;;
esac 
H=$1 #assegniamo a H il valore di $1
#--------------------------------------------------------------------#

#CONTROLLO CHE IL SECONDO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO MAGGIORE DI 1 E MINORE DI 255
#------------------------------------------------------------------#
case $2 in 
*[!0-9]*) echo Errore: $2 non numerico o non positivo
            exit 4;;
*)         if test $2 -le 0 -o $2 -gt 255
            then echo Errore:  il numero è uguale a zero oppure è maggiore di 255
            exit 5
            fi         
esac 
K=$2 #assegniamo a K il valore di $2
#--------------------------------------------------------------------#

#facciamo shift due volte per togliere i primi 2 parametri e andiamo a controllare gli altri N parametri
shift
shift

for i 
do 
    #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 6
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 7;;
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
    FCR.sh $G $H $K #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!


