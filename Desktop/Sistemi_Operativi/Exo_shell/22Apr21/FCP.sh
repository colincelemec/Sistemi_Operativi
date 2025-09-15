#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO

case $# in 
0|1|2|3) echo ERRORE SUL NUMERO DI PARAMETRI. CI VUOLE ALMENO 4 PARAMETRI
         exit 1;;
    *)   echo DEBUG-OK: NUMERO DI PARAMETRI GIUSTO E ORA POSSIAMO PROSEGUIRE;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0
            then echo Errore: il numero è uguale a zero
            exit 3
            fi;;
esac
W=$1 #assegniamo a W il valore di $1

#CONTROLLO CHE IL SECONDO PARAMETRO SIA UNA SEMPLICE STRINGA
case $2 in 
        */*) echo Errore : $2 contiene il carattere '/' quindi non è una stringa
                exit 4;;
        *);;
esac
S=$2 #assegniamo a S il valore di $2 
#facciamo un shift due volte per andare a lavorare sugli altri Q parametri
shift
shift 

for G 
do 
    #soliti controlli su nome assoluto e directory traversabile
        case $G in 
        /*) if test ! -d $G -o ! -x $G 
            then
                echo Errore: $G non directory o non attraversabile
                exit 5
            fi;;
        *)  echo Errore: $G non nome assoluto
            exit 6;;
        esac 
done

#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/nomiAssoluti #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G $S /tmp/nomiAssoluti #Invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

#si deve ritornare sullo stdout il numero totale delle directory trovate globalmente

echo il numero totale delle directory trovate è : `wc -l < /tmp/nomiAssoluti`
 
conteggio=`wc -l < /tmp/nomiAssoluti`

if test $conteggio -ge $W 
then 
    #chiediamo all'utente col suo proprio nome un numero intero X compreso fra 1 e W
    echo -m "Colince, dammi un numero X compreso fra 1 e $W: "
    read X
    #controlliamo se X è un numero ed è compreso fra 1 e $W
    case $X in 
    *[!0-9]*) echo Errore: $X non numerico o non positivo
              rm /tmp/nomiAssoluti;;

    *)  if test $i -eq 0
        then echo Errore: parametro $i uguale a zero
            rm /tmp/nomiAssoluti
        fi;;
    esac

    if test $X -ge 1 -a $X -le $W #se il numero è compreso fra 1 e W
    then 
        #si deve selezionare la directory corrisponde al numero X e riportare su stdout il suo nome assoluto
        nomeAssoluto=head -$X < /tmp/nomiAssoluti | tail -1
        echo il suo nome assoluto è : $nomeAssoluto
    fi 
fi 

rm /tmp/nomiAssoluti





