#!/bin/sh
#Soluzione della prova del 14 Luglio 2021

#Controllo sul numero dei parametri Q >=2 e quindi Q+2 >= 4

case $# in 
0|1|2|3) echo Errore: Voglio un numero maggiore o uguale a 4, mentre ne hai passati $#
         exit 1;;
*)       echo DEBUG-OK: da qui in poi proseguiamo con $# parametri;;
esac

#dobbiamo isolare gli ultimi due parametri e intanto facciamo i controlli 
num=1 #la variabile num ci serve per capire quando abbiamo trovato il penultimo e l'ultimo carattere
params= #la variabile params ci serve per accumulare i parametri a parte gli ultimi due

#in $* abbiamo i nomi delle gerarchie e i due numeri interi
for i 
do 
    if test $num -lt `expr $# - 1` #ci serve per non considerare gli utimi due parametri che somo i numeri
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
        params="$params $i" #se i controlli sono andati bene memorizzino il nome della lista params
    else
    #abbiamo individuato gli ultimi due parametri e quindi facciamo il solito controllo numerico o strettamente positivo 
    #controllo penultimo e ultimo

    case $i in 
    *[!0-9]*) echo Errore: $i non numerico o non positivo
              exit 4;;
    *)  if test $i -eq 0
        then echo Errore: parametro $i uguale a zero
            exit 5
        fi;;
    esac
    #se i controlli sono andati bene salviamo il penultimo e ultimo parametro
    if test $num -eq `expr $# - 1`
    then 
        H=$i #H nome indicato nel testo
    else 
        M=$i #M nome indicato nel testo
    fi
    num=`expr $num + 1` #incrementiamo il contatore del ciclo sui parametri
done 

#controlli sui parametri finito possiamo passare alle Q fasi, dopo aver settato il path
PATH=`pwd`:$PATH 
export PATH 

#ora in $params abbiamo solo i nomi delle gerarchie
for G in $params  #G nome indicato nel testo
do 
    echo DEBUG-fase per $G 
    FCR.sh $G $H $M #invochiamo il file ricorsivo
done 
