#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore sul numero di parametri. Ci vuole almeno 3 parametri
        exit 1;;
*)      echo DEBUG-OK: numero di parametri giusto e ora in poi proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO

case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0
            then echo Errore: primo parametro uguale a zero
            exit 3
            fi;;
esac 

Y=$1 #asseginamo a Y il valore del primo parametro

shift #faciamo un shift per andare sugli altri N parametri

#CONTROLLO SUGLI N ALTRI PARAMETRI
for i
do 
    #soliti controlli su nome assoluto e directory traversabile
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

#settiamo la variabile PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/conta$$ #creiamo/azzeriamo il file temporanea

#invocazione ricorsiva
for G 
do 
    echo DEBUG-FASE per $G 
    FCR.sh $G $Y /tmp/conta$$ #chiamata ricorsiva
done 

#riportiamo sullo standard output il numero di file creati globalmente
echo il numero di file creati globalmente è : `wc -l < /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
    echo creato il file: $i 
    case $i in 
    *NO*) echo ile file originale non conteneva almeno 5 linee e quindi il file creato è vuoto;;
    *)    echo il contenuto del file è il seguente
          echo ===
          cat < $i 
          echo ===
          ;;
    esac
done 

rm /tmp/conta$$ #cancelliamo il file ricorsivo




