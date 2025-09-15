#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
 case $# in 
 3) echo DEBUG-OK: Numero di parametri giusto;;
 *) echo Errore sul numero di parametri. Ci vuole esttamente 2 parametri
    exit 1;,
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

 #CONTROLLO CHE IL SECONDO PARAMETRO SIA UN NOME RELATIVO SEMPLICE DI UNA DIRECTORY

 case $2 in 
 */*) echo Errore: $2 non in forma relativa semplice
 exit 3;;
 *) if test ! -d $2 -o ! -x $2
    then 
         echo Errore: $2 non directory o non attraversabile
         exit 4
    fi;;
 esac
 D=$2 #assegniamo a D il valore di $2

 #CONTROLLO CHE IL TERZO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
case $3 in 
*[!0-9]*) echo Errore: $3 non numerico o non positivo
            exit 5;;
*)         if test $3 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 6
            fi;;
esac 
N=$3 #assegniamo a N il valore di $3

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/conta$$ #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
    echo fase per $G
    FCR.sh $G $D $N /tmp/conta$$ #Invocazione ricorsiva

#Se siamo qui allora , la fase ricorsiva è finita!

#calcoliamo il numero di file trovati
FILES=`wc -l < /tmp/conta$$`
if test $FILES -ge 1 #se abbiamo trovato almeno un file che soddisfa la specifica 
then 
    #invochiamo la parte c 
    main `cat /tmp/conta$$` $N 
else 
    echo Non abbiamo trovato nessun file che lo soddisfa
fi 


rm /tmp/conta$$ #cancelliamo la variabile temporanea




