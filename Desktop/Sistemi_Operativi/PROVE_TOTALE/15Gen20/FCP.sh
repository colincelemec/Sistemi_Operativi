#!/bin/sh
#file principale FCP.sh

#CONTROLLO SUL NUMERO DI PARAMETRO
case $# in 
0|1|2) echo Errore sul numero di parametri. Ci vuole almeno 3
        exit 1;;
    *)  echo DEBUG-OK: Numero di parametri giusto;;
esac 


#CONTROLLO SUL TIPO DI PARAMETRI

#CONTROLLO CHE IL PRIMO PARAMETRO SIA UN NUMERO INTERO STRETTAMENTE POSITIVO
#------------------------------------------------------------------#
case $1 in 
*[!0-9]*) echo Errore: $1 non numerico o non positivo
            exit 2;;
*)         if test $1 -eq 0
            then echo Errore:  il numero è uguale a zero
            exit 3
            fi;;
esac 
H=$1 #assegniamo a H il valore di $1
#--------------------------------------------------------------------#

shift #facciamo un shift per elimnatre il primo parametro e andiamo a controllare gli altri N

for i 
do 
    #CONTROLLO CHE I PARAMETRI SIANO NOMI ASSOLUTI DI DIRECTORY
        case $i in 
        /*) if test ! -d $i -o ! -x $i 
            then
                echo Errore: $i non directory o non attraversabile
                exit 4
            fi;;
        *)  echo Errore: $i non nome assoluto
            exit 5;;
        esac 
done 

#controllo sul numero e tipo di parametri finiti



#SETTIAMO LA VARIABILE PATH

PATH=`pwd`:$PATH #settaggio plausibile!
export PATH

n=1 #serve per dare un nome diverso ai file temporanei e, volendo, per dire in che fase siamo
for G	#G nome indicato nel testo
do
	> /tmp/nomiAssoluti-$n 	#creiamo/azzeriamo ogni file temporaneo, usando come parte iniziale del nome quello specificato nel testo!
	echo DEBUG-fase $n per la gerarchia $G
	#invochiamo il file comandi ricorsivo con la gerarchia, il numero X e il file temporaneo corrente
	FCR.sh $G $H /tmp/nomiAssoluti-$n   
	#la specifica indicava che doveva essere riportato su standard output il conteggio di OGNI SINGOLA gerarchia!
    FILES=`wc -l < /tmp/nomiAssoluti-$n`
	echo Il numero di file trovati in questa fase e\' $FILES 
    if test `expr $FILES % 2` #se tale numero è pari
    then
        #invocazione della parte C
        echo INVOCHIAMO LA PARTE C
        #main `cat /tmp/nomiAssoluti-$n`  
    fi 
	n=`expr $n + 1`		#incrementiamo il contatore
done

#Se siamo qui allora , la fase ricorsiva è finita!


#cancelliamo tutti i file temporanei
rm /tmp/nomiAssoluti-*       #possiamo usare questa forma sintetica invece che rifare un for con il comando rm applicato ad ogni file temporaneo

