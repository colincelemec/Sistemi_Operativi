#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2|3) echo Errore sul numero di parametri perché ci vuole almeno 4 parametri!!
            exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto e ora in poi possiamo proseguire;;
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
K=$2 #assegniamo a K il valore di $2
#--------------------------------------------------------------------#

#facciamo un shift due volte per andare a controllare gli atri N parametri
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