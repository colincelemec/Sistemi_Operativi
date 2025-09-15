#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo errore sul numero di parametri.Ci vuole almeno 3
        exit 1;;
*)      echo DEBUG-OK: numero di parametri corretti e ora proseguiamo;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CODICE CHE SEPARA I PRIMI N PARAMETRI DAL L'ULTIMO
count=1 #la variabile count serve per capire quando abbiamo trovato l'ultimo parametro
params= #per accumulare i primi N parametri tranne l'ultimo
for i 
do 
    if test $count -ne $#
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
        params="$params $i" #inseriamo il parametro nella lista
    else
        #controllo sull'ultimo parametro che deve essere un simplice stringa
        case $i in 
        */*) echo Errore : $i contiene il carattere '/' quindi non è una stringa
                exit 4;;
        *);;
        esac
        S=$i #mettiamo nella variabile S il valore di $i
    fi 
        count=`expr $count + 1`
done 


#settiamo la variabile PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

>/tmp/conta$$ #creaimo / azzeriamo il file temporameo

#Invocazione ricorsiva 

for G in $params
do 
if test -d $G -a -x $G 
then 
    echo DEBUG-fase per $G
    FCR.sh $G $S /tmp/conta$$ #chiamata ricorsiva
fi 
done 

#Qui siamo , siamo alla fine della chiamata ricorsiva

#Riportiamo su stdout il numero totale di file trovati globalmente

echo il numero totale di file trovati globalmente è `wc -l < /tmp/conta$$`
c=0 #variabile che serve nel for successivo per capire se l'elemento corrente del for è una lunghezza o il nome assoluto di un file

for i in `cat /tmp/conta$$`
do 
    #riportiamo la lunghezza in caratteri sullo stdout e il nome assoluto
    c=`expr $c + 1`
    echo il nome assoluto del file trovato è $i e la sua lunghezza in caratteri  è $c 
    #chiediamo all'utente se vuole ordinare il file o no
    echo -m "vuoi ordinare il file $i ?"
    read risposta
    case risposta in 
    si) #riportiamo sullo stdout il file ordinato secondo l'ordine alfabetico
        sort -f $i;;
    *)  echo Va bene! Non vuoi ordinare questo file;;
    esac 
done 

rm /tmp/conta$$ #cancelliamo il file temporaneo
