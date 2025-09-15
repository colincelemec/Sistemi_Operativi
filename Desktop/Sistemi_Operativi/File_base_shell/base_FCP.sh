#!/bin/sh
#file comandi FCP.sh

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
            then echo Errore: secondo parametro $2 uguale a zero
            exit 6
            fi;;
esac 
#--------------------------------------------------------------------#

#CONTROLLO CHE SIA UN SINGOLO CARATTERE
case $C in 
?) #DA COMPLETARE
*) echo allora non è un 

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
#---------------------------------------------------------------------------------------------------------------------------#

#settiamo la variabile PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

echo Beginc.sh: Stiamo per esplorare la directory $d
Cercafile.sh $d $f # invocazione file comandi ricorsivo!
echo HO FINITO TUTTO # azioni conclusive!