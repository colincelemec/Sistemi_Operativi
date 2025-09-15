#!/bin/sh
#file comandi principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
#il numero di parametri deve essere esattamente 3
if test $# -ne 3
then 
    echo Errore sul numero di parametri: Ci vuole esattamente 3 parametri
    exit 1
fi 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA IL NOME ASSOLUTO DI UNA DIRECTORY

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
case $2 in 
*[!0-9]*) echo Errore: $2 non numerico o non positivo
            exit 4;;
*)         if test $2 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 5
            fi;;
esac 

K=$2 #assegniamo a K il valore di $2

#CONTROLLO CHE IL TERZO PARAMETRO SIA UN SINGOLO CARATTERE
case $3 in
?) ;;
*)      echo Errore: $3 non singolo carattere
        exit 6;;
esac

Cx=$3 #assegniamo a Cx il valore di $3

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH
PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/conta$$ #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
echo fase per $G
FCR.sh $G $K $Cx /tmp/conta$$ #Invocazione ricorsiva

#Se siamo qui allora , la fase ricorsiva è finita!

#si deve invocare la parte C passando come parametri tutti i nomi assoluti dei file Fi trovati e Cx

#calcoliamo il numero di file che soddisfano la specifica

FILES=`wc -l < /tmp/conta$$`

if test $FILES -ge 1
then 
    echo abbiamo trovati i file che soddisfano la specifica
    echo sono i seguenti file:
    cat /tmp/conta$$
    #invozazione della parte C
    #main `cat /tmp/conta$$` $Cx 
else 
    echo NON abbiamo trovato alcun file che soddisfa la specifica
fi 

rm /tmp/conta$$ #cancelliamo il file temporaneo
 
