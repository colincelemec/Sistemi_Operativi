#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo ERRORE SUL NUMERO DI PARAMETRI. CI VUOLE ALMENO 3 PARAMETRI
        exit 1;;
    *) echo DEBUG-OK: NUMERO DI PARAMETRI GIUSTO E ORA PROSEGUIMO;;
esac

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN SINGOLO CARETTERE
case $1 in
?) ;;
*)      echo Errore: $1 non singolo carattere
        exit 2;;
esac
C=$1 #assegniamo alla variabile C il valore di $1

shift #facciamo un shift per andare a lavorare sugli altri N parametri

#CONTROLLO CHE I N PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY 

for i 
do 
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

#SETTIAMO LA VARIABILE PATH
PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/nomiAssoluti #creiamo/azzeriamo la variabile temporanea

#INVOCAZIONE RICORSIVA
for G 
do 
    echo fase per $G
    FCR.sh $G $C /tmp/nomiAssoluti #invocazione ricorsiva
done 

#Se siamo qui allora , la fase ricorsiva è finita!

#riportare su stdout il numero totale delle directory trovate globalmente

echo il numero totale delle directory trovate globalmente è : `wc -l < /tmp/nomiAssoluti`

#riportare su stdout i nomi assoluti delle directory trovate 

utente=Elena
for i in `cat /tmp/nomiAssoluti`
do 
    echo directory trovata $i 
    echo -m "$utente , vuoi visualizzare il contenuto della directory $i compresi anche gli elementi nascosti ?"
    read answer 
    case answer in 
    si) #visualizza il contenuto della directory
        echo il contenuto della directory $i è
        ls -la $i;;
    no);;
    esac 
done 

rm /tmp/nomiAssoluti 
