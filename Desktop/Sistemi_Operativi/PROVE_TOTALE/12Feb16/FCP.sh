#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO

#la parte shell prevede 2 parametri

case $# in 
2) echo DEBUG-OK: numero di parametri giusto;;
*) echo Errore sul numero di paramatri. Ci vuole esattamente 2 parametri!
    exit 1;;
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
G=$1 #assegno a G il valore d $1

#CONTROLLO CHE IL SECONDO PARAMETRO SIA UN SINGOLO CARATTERE
case $2 in
[a-z]) ;;
*)      echo Errore: $2 non singolo carattere alfabetico minuscolo
        exit 4;;
esac
Cx=$2 #assegniamo a Cx il valore di $2

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/conta$$ #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA 
echo fase per $G
FCR.sh $G $Cx /tmp/conta$$ #Invocazione ricorsiva

#Se siamo qui allora , la fase ricorsiva è finita!

#calcoliamo il numero di file che soddisfano la specifica
FILES=`wc -l < /tmp/conta$$`

if test $FILES -ge 2 #se abbiamo trovato almeno 2 file, allora
then 
    #si deve invocare la parte C passando come parametri i nomi dei file trovati e il carattere Cx
    echo abbiamo trovati i seguenti file che soddisfano la specifica
    cat /tmp/conta$$
    main `cat /tmp/conta$$` Cx 
fi 


rm /tmp/conta$$ #cancelliamo il file temporaneo

