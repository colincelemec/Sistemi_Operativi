#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore sul numero di parametri. ci vuole almeno 3 parametri
       exit 1;;
*)      echo DEBUG-OK: Numero di parametri giusto e qui in poi proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI 
#CONTROLLO CHE I PRIMI N PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY

#CODICE CHE SEPARA I PRIMI N PARAMETRI DAL L'ULTIMO
count=1 #la variabile count serve per capire quando abbiamo trovato l'ultimo parametro
params= #per accumulare i primi N parametri tranne l'ultimo
for i 
do 
    if test $count -ne $#
    then 
        #soliti controlli su nome assoluto e directory traversabile
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 2
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 3;;
        esac 
        params="$params $i" #inseriamo il parametro nella lista
    else
        #controllo sull'ultimo parametro che deve essere un numero intero strettamente positivo
         case $i in 
         *[!0-9]*) echo Errore: $i non numerico o non positivo
                exit 4;;
         *)  if test $i -eq 0
                then echo Errore: parametro $i uguale a zero
                exit 5
             fi;;
    esac
        X=$i #
    fi 
        count=`expr $count + 1`
done 

#settiamo la variabile PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/conta$$ #creiamo/azzeriamo il file temporaneo


for G in $params
do 
    echo DEBUG-FASE per $G 
    FCR.sh $G $X /tmp/conta$$ #invochiamo il file ricorsivo 
done 

#qui siamo al termine di tutte le N fasi
#riportiamo sullo stdout il numero totale delle directory trovate globalmente che soddisfano la specifica 

echo il numero totale delle file trovate globalmente e che soddisfano la specifica  è : `wc -l < /tmp/conta$$`

for i in `cat /tmp/conta$$`
do 
    #stampiamo il nome assoluto del file trovato
    echo trovato il file $i
    #riportiamo la linea X-iesima del file corrente a partire dalla fine del file
    tail -$X $i | head -1 
done 

rm /tmp/conta$$ #azzeriamo il file temporaneo

