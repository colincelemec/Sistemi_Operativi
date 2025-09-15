#!/bin/sh
#File comandi ricorsivo che identifica un certo livello di una gerarchia, supposto che esista

cd $1

#il primo livello verra' contato come livello 1
conta=`expr $3 + 1`  #nota bene questa variabile conta e' una variabile conta LOCALE a questo script

#echo conta uguale $conta 

#verifichiamo se $conta e' il livello che stiamo cercando
if test $conta -eq $2 
then
	echo DEBUG-TROVATA directory `pwd` che si trova a livello GIUSTO $conta
	pwd >> $4	#salviamo il nome assoluto della directory nel primo file temporaneo
	#N.B. basta usare il comando pwd: NON serve echo `pwd`!
	#quindi bisogna raccogliere i nomi di tutti i file leggibili con lunghezza non vuota 
	for F in *	#F variabile indicata dal testo
	do
		if test -f $F -a -r $F
		then 
			if test `wc -c < $F` -ne 0
			then 
				echo $F >> $5 #salviamo il nome relativo del file nel secondo file temporaneo
			fi
		fi 
	done
fi 

for i in *
do
	if test -d $i -a -x $i
	then
		#echo RICORSIONE in `pwd`/$i  con terzo paramentro uguale a $conta
		$0 `pwd`/$i $2 $conta $4 $5
    	fi
done
