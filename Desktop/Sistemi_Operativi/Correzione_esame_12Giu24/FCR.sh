#!/bin/sh
# Versione funzionante della parte shell(FCR.sh) dell'esame del 12 Giugno 2024


Cont=0		#variabile  per capire se abbiamo trovato almeno un file che soddisfa la specifica (leggibile) e quindi la directory e' giusta!
NL=		#variabile di appoggio che ci servira' per calcolare il numero di linee dei file leggibili
files=  	#variabile di appoggio che ci servira' per raccogliere i nomi dei file

cd $1

case $1 in
*/$2?$2?$2) #la dir ha il nome giusto e quindi ora verifichiamo che abbia almeno un file leggibile
	for item in *
	do
        if test -f $item -a -r $item #se e un file ed e leggibie
		then
			NL=`wc -l < $item`
			if test $NL -ne 0 -a `expr $NL % 2` -eq 0   #se ha una lunghezza in linee diversa da zero e pari! 
			then
				files="$files $item"      #salviamo il nome nella variabile
				Cont=`expr $Cont + 1`	#e incrementiamo il conteggio 
			else
				echo DEBUG-il file $item ha lunghezza in linee $NL e quindi 0 o NON pari
			fi
		fi
	done

	if test $Cont -ge 1 
	then
		#se abbiamo trovato almeno un file dobbiamo stampare il nome assoluto della dir corrente e invocare la parte C
		echo DIRECTORY TROVATA CON NOME ASSOLUTO `pwd`
		echo DEBUG-CHIAMIAMO LA PARTE C CON $files
        	#main $files
	else
		echo LA DIRECTORY CON NOME ASSOLUTO `pwd` NON CONTIENE ALCUN FILE LEGGIBILE DI DIMENSIONE NON NULLA O CON LUNGHEZZA IN LINEE PARI!
	fi;;
esac

for i in *
do
        if test -d $i -a -x $i 
        then
                #invocazione ricorsiva
                FCR.sh `pwd`/$i $2 
        fi
done
