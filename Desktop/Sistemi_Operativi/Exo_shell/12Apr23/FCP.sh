#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2|3) echo Errore sul numero di parametri. Ci vuole almeno 4: 2 stringhe e 2 nomi assoluti di directory
            exit 1;;
    *)   echo DEBUG-OK: Numero di parametri giusto: Ora in poi proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UNA SEMPLICE STRINGA
#------------------------------------------------------------------#
case $1 in 
        ?) echo Errore : $1 contiene un solo carattere, allora non è una stringa!!
                exit 2;;
        *);;
esac
S1=$1 #assegniamo a S1 il valore di $1

#CONTROLLO CHE IL SECONDO PARAMETRO SIA UNA SEMPLICE STRINGA
#------------------------------------------------------------------#
case $2 in 
        ?) echo Errore : $2 contiene un solo carattere, allora non è una stringa!!
                exit 3;;
        *);;
esac
S2=$2 #assegniamo a S2 il valore di $2

#facciamo un shift due volte per andare a lavorare sugli altri N parametri per togliere il primo e il secondo parametro

shift 
shift 

for i 
do 
    #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 4
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 5;;
        esac 
done 

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

> /tmp/nomiAssoluti-1 #creiamo/azzeriamo la prima variabile temporanea 
> /tmp/nomiAssoluti-2 #creiamo/azzeriamo la seconda variabile temporanea

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G $S1 $S2 /tmp/nomiAssoluti-1 /tmp/nomiAssoluti-2 #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

TOT1=`wc -l < /tmp/nomiAssoluti-1`
TOT2=`wc -l < /tmp/nomiAssoluti-2`

echo il numero totale dei file trovati con estensione $1 è $TOT1 
echo il numero totale dei file trovati con estensione $2 è $TOT2

if test $TOT1 -le $TOT2 
then 
    #sei deve chiedere all'utente chiamandolo con il proprio nome
    echo Colince, devi scegliere un numero compreso X compreso fra 1 e $TOT1 : 
    read $X 

    #CONTROLLO CHE X SIA UN NUMERO INTERO STRETTAMENTE POSITIVO E SIA COMPRESO FRA 1 E TOT1

    case $X in 
    *[!0-9]*) echo Errore: $X non numerico o non positivo
            exit 6;;
    *)         if test $X -lt 1 -o $X -gt 100
            then echo Errore:  il numero non è compreso tra 0 e 100
            exit 7
            fi;;         
    esac
fi 

    #si deve selezionare il nome del file leggibile con estensione S1 e S2 corrispondente al numero X fornito

    for F in `cat /tmp/nomiAssoluti-1`
    do 
        head -$X $F | tail -1 
        
    done 

    for F in `cat /tmp/nomiAssoluti-2`
    do 
        head -$X $F | tail -1 

    done 


rm /tmp/nomiAssoluti-1 #cancelliamo la variabile temporanea
rm /tmp/nomiAssoluti-2 #cancelliamo la variabile temporanea
