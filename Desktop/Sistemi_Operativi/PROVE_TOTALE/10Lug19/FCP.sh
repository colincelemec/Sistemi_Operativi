#!/bin/sh
#file comandi FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO

case $# in 
0|1|2) echo Errore sul numero di parametri. Ci vuole almeno 3 parametri.
        exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto. Possiamo proseguire;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN SINGOLO CARATTERE
case $1 in
?) ;;
*)      echo Errore: $1 non singolo carattere
        exit 2;;
esac
Cz=$1 #mettiamo dentro Cz il valore di $1

shift #facciamo un shift per andare a controllare gli altri N parametri

for i 
do 
    #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 3
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 4;;
        esac 
done 

#controllo sul numero e tipo di parametri finiti

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/nomiAssoluti #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G $Cz /tmp/nomiAssoluti #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

FILES=`wc -l < /tmp/nomiAssoluti` #calcoliamo il numero di file trovati che soddisfano la specifica
echo il numero di file trovati che soddifano la specifica è $FILES 

#invochiamo la parte c 
echo IINVOCAZIONE DELLA PARTE C
#main $FILES $Cz 

rm /tmp/nomiAssoluti #cancelliamo il file temporaneo
