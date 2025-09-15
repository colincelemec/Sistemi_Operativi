#!/bin/sh
#Versione funzionante della parte shell(FCP.sh) dell'esame del 12 Giugno 2024

case $# in
0|1|2) echo Errore: numero parametri $# quindi pochi parametri. Usage is $0 G1 G2 ... C
         exit 1;;
*)       echo DEBUG-OK: da qui in poi proseguiamo con $# parametri ;;
esac

#dobbiamo isolare l'ultimo parametro e intanto facciamo i controlli
num=1   #la variabile num ci serve per capire quando abbiamo trovato l'ultimo parametro
params= #la variabile params ci serve per accumulare i parametri a parte l'ultimo
#in $* abbiamo i nomi delle gerarchie e il carattere 
for i
do
	if test $num -ne $# #ci serve per non considerare l'ultimo parametro che e' il carattere 
	then
	#soliti controlli su nome assoluto e directory traversabile
		case $i in
	        /*)     if test ! -d $i -o ! -x $i
		        then
				echo $i non directory o non attraversabile
				exit 2
			fi;;
		*)      echo $i non nome assoluto; exit 3;;
		esac
		params="$params $i" #se i controlli sono andati bene memorizziamo il nome nella lista params 
	else
	#abbiamo individuato l'ultimo parametro e quindi facciamo il controllo che sia un singolo carattere
	case $i in
	?) echo DEBUG-$i singolo carattere ;;
	*) echo ERRORE: ultimo parametro $i NON singolo carattere
	   exit 3;;
	esac
	car=$i	#nome variabile specificato nel testo
	fi
	num=`expr $num + 1` #incrementiamo il contatore del ciclo sui parametri
done

#controlli sui parametri finiti possiamo passare alle Q fasi, dopo aver settato il path
PATH=`pwd`:$PATH
export PATH

for dirAss in $params
do
        echo fase per $dirAss	
	#invochiamo il file comandi ricorsivo con la gerarchia e l'ultimo parametro (cioe' il carattere) 
	   FCR.sh $dirAss $car 
done
