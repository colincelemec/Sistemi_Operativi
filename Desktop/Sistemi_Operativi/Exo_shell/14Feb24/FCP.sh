#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO

case $# in 
0|1|2|3) echo ERRORE SUL NUMERO DI PARAMETRI. Ci vuole almeno 4
        exit 1;;
    *)   echo DEBUG-OK: Numero di parametri giusto. Possiamo proseguire;;
esac


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
X=$2 #assegniamo a N il valore di $2

#facciamo due shift per andare a lavorare sugli altri N parametri

shift #facciamo un shift per lavorare sugli ultimi parametri
shift

#CONTROLLO CHE GLI ULTIMI N PARAMETRI SIANO SEMPLICI STRINGHI
#------------------------------------------------------------------#
for S
do
    case $S in 
            ?) echo Errore : $S contiene un solo carattere, allora non è una stringa!!
                exit 5;;
            *);;
    esac
     
done 

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

#INVOCAZIONE RICORSIVA
echo fase per $G
FCR.sh $G $X $*  #Invocazione ricorsiva