#!/bin/sh
#Soluzione della Prova dell'11 Settembre 2024
#File comandi principale da invocare con numerostrettpos dirass1 dirass2 ...
#ATTENZIONE IL FILE FCP.sh DEVE ESSERE UN FILE IN CUI IL DIRITTO DI ESECUZIONE ALMENO PER L'UTENTE RISULTA SETTATO!

case $# in
0|1|2) echo Errore: voglio un numero di parametri maggiore o uguale a 3, mentre ne hai passati $# - Usage is $0 numerostrettpos dirass1 dirass2 ...
       exit 1;;
*)     echo DEBUG-da qui in poi proseguiamo con $# parametri ;;
esac

#Controllo primo parametro: deve essere un numero strettamente maggiore di 0 (con case)
case $1 in
*[!0-9]*) echo Errore: $1 non numerico o non positivo
          exit 2;;
*)        if test $1 -eq 0   	#sbagliato
          then
                  echo Errore: $1 uguale a 0 
                  exit 3
          else
                  echo DEBUG-primo parametro giusto $1
          fi;;
esac

X=$1    #salviamo il numero nella variabile indicata dal testo 

#quindi ora possiamo usare il comando shift per eliminare il numero 
shift

#ora in $* abbiamo solo i nomi delle gerarchie e quindi possiamo fare i controlli sui nomi assoluti e sulle directory in un for
for G
do
        case $G in
        /*) if test ! -d $G -o ! -x $G
            then
            echo Errore: $G non directory o non traversabile
            exit 4
            fi;;
        *)  echo Errore: $G non nome assoluto; exit 5;;
        esac
done

#controlli sui parametri finiti possiamo passare alle Q fasi, dopo aver settato il path
PATH=`pwd`:$PATH
export PATH

> /tmp/nomi-1 #creiamo/azzeriamo il primo file temporaneo 
> /tmp/nomi-2 #creiamo/azzeriamo il secondo file temporaneo 
#NOTA BENE: DUE file temporanei, uno per i nomi assoluti delle directory trovate e una per i nomi relativi semplici dei file trovati in tali dir!

for G 	#NOME STABILITO DAL TESTO
do
        echo DEBUG-fase per $G	
	#invochiamo il file comandi ricorsivo con la gerarchia, il numero X, il livello corrente che inizialmente poniamo a 0 e i due file temporanei
	FCR.sh $G $X 0 /tmp/nomi-1 /tmp/nomi-2	
	#il primo livello (di ogni gerarchia G) verra' contato come livello 1 dato che verra' subito incrementato nel file comandi ricorsivo: se invece avessimo voluto contarlo come livello 0 allora il valore iniziale del terzo parametro doveva essere -1
done

#terminate tutte le ricerche ricorsive cioe' le Q fasi
#N.B. Andiamo a contare le linee del file /tmp/nomi-1
NTot=`wc -l < /tmp/nomi-1` 
echo Il numero totale di directory che soddisfano la specifica = $NTot
echo "DEBUG-Le directory che abbiamo trovato a livello $X sono le seguenti:"
cat /tmp/nomi-1

for d in `cat /tmp/nomi-1`	#per ogni directory trovata
do
	cd $d	#ci spostiamo nella directory trovata
	#chiamiamo la parte in C passando il nome della directory corrente e i nomi dei file trovati in tutte le directory
	echo DEBUG-chiamo la parte in C con $d `cat /tmp/nomi-2`
	11Set24 $d `cat /tmp/nomi-2`
done

#da ultimo eliminiamo i due file temporanei
rm /tmp/nomi-1
rm /tmp/nomi-2
