#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO

case $# in 
0|1|2)  echo Errore sul numero di parametri. Ci vuole almeno 3 parametri
        exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto. Possiamo proseguire;;
esac 

#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL  PRIMO PARAMETRO SIA UN SINGOLO CARATTERE ALFABETICO MINUSCOLO
case $1 in
	?) echo $1 singolo carattere ora controlliamo sia un alfabetico minuscolo
	case $1 in
		[!a-z]) echo $1 non alfabetico minuscolo
		  	exit 2;;
		*) echo $1 carattere alfabetico minuscolo;;
	esac;;
	*)	echo $1 sbagliato dato che non singolo carattere
		exit 3;;
esac
C=$1 #mettiamo dentro C il valore di $1

#facciamo un shift per togliere il primo parametro e andare al lavorare sugli altr N parametri
shift 

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
    FCR.sh $G $C /tmp/nomiAssoluti #Invocazione ricorsiva
done 


#Se siamo qui allora , la fase ricorsiva è finita!

N=`wc -l < /tmp/nomiAssoluti`
echo il numero di file trovati è $N 

if test $N -ge 2
then 
    echo invochiamo la parte C passando come parametri i nomi assoluti di tutti i file trovati globalmente
    main `cat /tmp/nomiAssoluti`
fi 


rm /tmp/nomiAssoluti