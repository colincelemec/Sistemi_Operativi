#!/bin/sh
#FCP.sh

#controllo sul numero di parametri N >= 2 e quindi N+1 >= 3

case $# in 
0|1|2) echo Errore: voglio un numero di parametri maggiore o uguale a 3
        exit 1;;
*)      echo DEBUG-OK: da qui in poi proseguiamo con $# parametri::
esac 

#Controllo sul primo parametro che deve essere un numero intero strettamente positivo

case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0
            then echo Errore: primo parametro $1 uguale a zero
            exit 3
            fi;;
esac 

X=$1 #salviamo il primo parametro
#quindi ora possiamo usare il comando shift
shift 

#Ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for

for i 
do 
    case $i in 
    /*) if test ! -d $i -o ! -x $i 
        then 
            echo Errore: $i non directory o non traversabile
            exit 4
        fi;;
    *)  echo Errore: $i non nome assoluto
        exit 5;;
    esac 
done 

#controllo sui parametri finiti possiamo passare alle N fasi , dopo aver settato il path
PATH=`pwd`:$PATH
export PATH 

> /tmp/conta$$ #creiamo/azzeriamo il file temporaneo

for i 
do 
    echo DEBUG-fase per $i 
    #invochiamo il file comandi ricorsivo
    FCR.sh $i $X /tmp/conta$$
done 

#terminate tutte le ricerche ricorsive cioè le N fasi

echo il numero di file totali che soddisfano la specifica = `wc -l < /tmp/conta$$`
for i in `cat /tmp/conta$$`
do 
    #stampiamo i nomi assoluti dei file trovati
    echo Trovato il file $i 
    #chiediamo all'utente il numero K per ogni file trovato
    echo -n "Dammi il numero K (strettamente maggiore di 0 e strettamente minore di $X): "
    read K
    #controllo se K è un numero

    expr $K + 0  > /dev/null 2>&1
	N1=$?
	if test $N1 -ne 2 -a $N1 -ne 3
	then 	#echo DEBUG-OK numerico $K #siamo sicuri che numerico
     		if test $K -le 0 -o $K -ge $X 
     		then 	echo Errore: $K non positivo o non minore di $X 
			rm /tmp/conta$$ #poiche' stiamo uscendo a causa di un errore, cancelliamo il file temporaneo!
          		exit 6
     		fi
	else
  		echo Errore: $K non numerico
		rm /tmp/conta$$ #poiche' stiamo uscendo a causa di un errore, cancelliamo il file temporaneo!
  		exit 7
	fi
    #selezioniamo direttamente la $K-iesima linea del file corrente
        head -$K $i | tail -1
done 

rm /tmp/conta$$ 