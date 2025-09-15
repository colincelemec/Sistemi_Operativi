#!/bin/sh
#Soluzione della Prova del 15 Febbraio 2017
#File comandi principale da invocare con dirass numerostrettpos (X)

#controllo sul numero dei parametri: devono essere esattamente 2
case $# in
2) 	echo DEBUG-OK numero di parametri = $#;;
*) 	echo Errore: voglio due parametri, mentre ne hai passati $# - Usage is $0 dirass numerostrettpos-X
	exit 1;;
esac

#Controllo primo parametro: deve essere un nome assoluto di una directory traversabile
case $1 in
/*) if test ! -d $1 -o ! -x $1
    then
    	echo Errore: $1 non directory o non traversabile
    	exit 2
    fi;;
*)  echo Errore: $1 non nome assoluto
    exit 3;;
esac

#Controllo secondo parametro (fatto con expr): deve essere un numero intero strettamente positivo
expr $2 + 0  > /dev/null 2>&1
N1=$?
if test $N1 -ne 2 -a $N1 -ne 3 
then 	echo DEBUG-OK numerico = $2 #siamo sicuri che numerico
     	if test $2 -le 0 
     	then 	echo Errore: $2 non strettamente positivo
       	  	exit 4
     fi
else
  	echo Errore: $2 non numerico
  	exit 5
fi

#controlli sui parametri finiti, settiamo la variabile PATH e la esportiamo
PATH=`pwd`:$PATH
export PATH

#invocazione del file comando ricorsivo
FCR.sh $*	#passaggio di tutti e due i parametri 
