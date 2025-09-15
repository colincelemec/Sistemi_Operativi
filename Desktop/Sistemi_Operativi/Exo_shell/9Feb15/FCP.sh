#!/bin/sh
#file principale FCP.sh

#controllo sul numero di parametri che deveno essere esattamente 2

case $# in 
0|1) echo Errore: numero parametri is $# quindi pochi parametri.
     exit 1;;
2)   echo DEBUG-OK: da qui in poi proseguiamo con $# parametri;;
*)   echo Errore: troppi parametri
     exit 2;;
esac 


#controllo sul primo parametro che deve essere nome assoluto di una directory e traversabile

case $1 in 
/*) if test ! -d $1 -0 ! -x $1
    then 
        echo Errore: $1 non directory oppure non traversabile
        exit 3
    fi;;
*)  echo Errore: $1 non nome assoluto
    exit 4;;
esac

#controllo secondo parametro che deve essere un numero intero strettamente positivo

case $2 in 
*[!0-9]*) echo Errore: $2 non numerico o non positivo
            exit 5;;
*)         if test $2 -eq 0
            then echo Errore: secondo parametro $2 uguale a zero
            exit 6
            fi;;
esac 

#settiamo la variabile PATH

PATH=`pwd`:$PATH
export PATH 
> /tmp/tmp$$ #creiamo il file temporaneo che passeremo poi come ultimo parametro al file comandi ricorsivo che servirò per raccogliere i nomi assoluti dei file trovati

FCR.sh $1 $2 /tmp/tmp$$

params= #variabile in cui inseriamo i nomi assoluti dei file trovati e i numero chiesti all'utente

for i in `cat /tmp/tmp$$`
do 
    params="$params $i" #inseriamo il nome assoluto che è l'elemento corrente del file temporameo
    #Il programma, per ognuno dei file trovati , deve richiedere all'utenteun numero X intero strettamente positivvo e minore di $2 
    echo -n "Dammi un numero intero strettamente possitivo e minore o uguale a $2 per il file $i: "
    read X
    #controlle X
    case $X in 
    *[!0-9]*) echo Errore: non numerico o non positivo
                rm /tmp/tmp$$ #poiché stiamo uscendo a causa di un errore, cancelliamo il file temporaneo
                exit 7;; 
    *)          if test $X -eq 0
                then 
                    echo Errore: non numerico o non positivo
                    rm /tmp/tmp$$ #poiché stiamo uscendo a causa di un errore, cancelliamo il file temporaneo
                    exit 8
                fi;;
    esac 
    if test $X -gt $2 
    then 
        echo Errore: Numero fornito $X non minore di $2
        rm /tmp/tmp$$ #cancelliamo il file temporaneo
        exit 9
    fi 
    #se arriviamo qui il numero inserito dall'utemte è corretto. cancelliamo il file temporaneo
    params="$params $X" #lo inseriamo nella variabile params
done 

#cancelliamo il file temporaneo dato che non ne abbiamo più bisogno
rm /tmp/tmp$$

#In fine , si deve invocare la parte C passando come parametri gli N nomi assoluti del file selezionati dei file F0. F1, ..., FN-1 intervallati dai numeri positivi chiesti all'utente
echo DEBUG-Sto pee chiamare la parte C a cui passano i parametri $params