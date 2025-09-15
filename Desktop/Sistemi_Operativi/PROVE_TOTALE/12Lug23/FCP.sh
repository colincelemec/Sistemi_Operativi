#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in
0| 1 |2) echo Errore nel numero di parametri.Ci vuole almeno 3.
    exit 1;;
*) echo DEBUG-OK: Numero di parametri Giusto. Proseguiamo ora in poi;;
esac

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO E STRETTAMNTE MINORE DI 200

case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0 -o $1 -ge 200
            then echo Errore:  il numero è uguale a zero oppure è maggiore di 200
            exit 3
            fi;;
esac 
X=$1 #assegniamo a X il valore di $1

shift 

for i 
do 
    #controllo che gli altri N parametri siano nomi assoluti di directory
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

>/tmp/nomiAssoluti #creaimo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G $X /tmp/nomiAssoluti #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

#si riporta sullo standard output il numero di file trovati e la lunghezza in linee massima
FILES=`wc -l < /tmp/nomiAssoluti` #per il numero totale di file trovati
echo il numero totale di file trovati è : $FILES
 

if test $FILES -ge 2 #se abbiamo trovato almeno due file
then 
    #invochiamo la parte C passando come parametro e il numero di file trovati 
    main $FILES `cat /tmp/nomiAssoluti`
fi 

rm /tmp/nomiAssoluti #cancelliamo la variabile temporanea
