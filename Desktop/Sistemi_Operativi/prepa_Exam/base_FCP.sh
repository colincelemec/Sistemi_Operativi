#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
#--------------------------------------#
case $# in # controllo numero parametri!
0) echo "Usage is: $0 [directory] file"
exit 1;;
1) d=`pwd`; f=$1;;
2) d=$1; f=$2;;
*) echo "Usage is: $0 [directory] file"
exit 1;; #uscita se errore!
esac
#------------------------------------------#

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PARAMETRO SIA UN NOME ASSOLUTO
#------------------------------------------------#
case $d in 
/*) ;;
*) echo Errore: $d non in forma assoluta
exit 2;; # valori diversi per maggiore usabilità!
esac
#--------------------------------------------------#

#CONTROLLO CHE IL PARAMETRO DATO SIA UN NOME RELATIVO SEMPLICE
#-------------------------------------------------#
case $f in 
*/*) echo Errore: $f non in forma relativa semplice
exit 3;;
*) ;;
esac
#-------------------------------------------------#

#CONTROLLO CHE IL PARAMETRO SIA UNA DIRECTORY E CHE SIA TRAVERSABILE
#--------------------------------------------------------------#
if test ! -d $d -o ! -x $d  #controllo dir e traversabile!
    then 
    echo Errore: $d non directory; exit 4
fi
#-----------------------------------------------------------------#

#CONTROLLO CHE IL PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
#------------------------------------------------------------------#
case $n in 
*[!0-9]*) echo Errore: $n non numerico o non positivo
            exit 5;;
*)         if test $n -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 6
            fi;;
esac 
#--------------------------------------------------------------------#

#CONTROLLO CHE IL  PARAMETRO SIA UN SINGOLO CARATTERE
case $1 in
?) ;;
*)      echo Errore: $1 non singolo carattere
        exit 2;;
esac
#CONTROLLO CHE SIA UNA SEMPLICE STRINGA
#------------------------------------------------------------------#
case $i in 
        ?) echo Errore : $i contiene un solo carattere, allora non è una stringa!!
                exit 4;;
        *);;
esac
#---------------------------------------------------------------------#
#CODICE CHE SEPARA I PRIMI PARAMETRI Q CHE SONO NOMI ASSOLUTI DI DIRECTORY DAGLI ULTIMI DUE CHE SONO NUMERI
#------------------------------------------------------------------------------------------------------------------#
#dobbiamo isolare gli ultimi due parametri e intanto facciamo i controlli 
num=1 #la variabile num ci serve per capire quando abbiamo trovato il penultimo e l'ultimo carattere
params= #la variabile params ci serve per accumulare i parametri a parte gli ultimi due

#in $* abbiamo i nomi delle gerarchie e i due numeri interi
for i 
do 
    if test $num -lt `expr $# - 1` #ci serve per non considerare gli utimi due parametri che somo i numeri
    then
        #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
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
#---------------------------------------------------------------------------------------------------------------------------#

#CODICE CHE SEPARA I PRIMI N PARAMETRI DAL L'ULTIMO
count=1 #la variabile count serve per capire quando abbiamo trovato l'ultimo parametro
params= #per accumulare i primi N parametri tranne l'ultimo
for i 
do 
    if test $count -ne $#
    then 
        #soliti controlli su nome assoluto e directory traversabile
        params="$params $i" #inseriamo il parametro nella lista
    else
        #controllo sull'ultimo parametro
        X=$i 
    fi 
        count=`expr $count + 1`
done 

#controllo sul numero e tipo di parametri FINITO



#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/nomiAssoluti #creiamo/azzeriamo la variabile temporanea


#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh ... ... #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

#QUESTO E UN PEZZO DI CODICE DA SCRIVERE NEL CASO DOVE ABBIAMO PIU FILE TEMPORANEI E VOGLIAMO SEPARARE
#---------------------------------------------------------------------------------------------------------------------------------------------#
n=1 #serve per dare un nome diverso ai file temporanei e, volendo, per dire in che fase siamo
for G	#G nome indicato nel testo
do
	> /tmp/nomiAssoluti-$n 	#creiamo/azzeriamo ogni file temporaneo, usando come parte iniziale del nome quello specificato nel testo!
	echo DEBUG-fase $n per la gerarchia $G
	#invochiamo il file comandi ricorsivo con la gerarchia, il numero X e il file temporaneo corrente
	FCR.sh $G $X /tmp/nomiAssoluti-$n   
	#la specifica indicava che doveva essere riportato su standard output il conteggio di OGNI SINGOLA gerarchia!
	echo Il numero di file trovati in questa fase e\' `wc -l < /tmp/nomiAssoluti-$n`
	n=`expr $n + 1`		#incrementiamo il contatore
done
#terminate tutte le ricerche ricorsive cioe' le Q fasi, dobbiamo procedere con il confronto richiesto
for file1 in `cat  /tmp/nomiAssoluti-1`	#per ogni file trovato nella prima gerarchia
do
	n=1 	#idem come prima: ATTENZIONE QUESTA INIZIALIZZAZIONE E' DA FARE ALL'INTERNO DI QUESTO CICLO!
	for i	#per ognuna delle gerarchie
	do
		if test $n -gt 1	#saltiamo il file temporaneo della prima gerarchia; N.B. in alternativa prima di iniziare questa parte di confronto si poteva usare il comando shift e quindi avere in $* solo le gerarchie a partire dalla G2
		then	
			for file2 in `cat  /tmp/nomiAssoluti-$n` #per ogni file trovato nelle altre gerarchie
			do
				#Stampa di DEBUG
				echo DEBUG-STIAMO PER CONTROLLARE il file $file1 e il file $file2
				if diff $file1 $file2 > /dev/null 2>&1	#basta usare il comando diff che riporta 0 se i file sono uguali, altrimenti un numero diverso da 0: da notare sia la ridirezione dello standard output che dello standard error su /dev/null
				then 	echo I file $file1 e $file2 sono uguali
				#else	echo I file $file1 e $file2 sono diversi 	#a seconda dei testi doveva essere riportato che i file fossero uguali o diversi e nel secondo caso era da fare questo else andando a non fare nulla nello then o mettendo una condizione inversa
				fi
			done 
		fi
		n=`expr $n + 1`		#incrementiamo il contatore
	done
done	

#cancelliamo tutti i file temporanei
rm /tmp/nomiAssoluti-*       #possiamo usare questa forma sintetica invece che rifare un for con il comando rm applicato ad ogni file temporaneo
#---------------------------------------------------------------------------------------------------------------------------------------------#

