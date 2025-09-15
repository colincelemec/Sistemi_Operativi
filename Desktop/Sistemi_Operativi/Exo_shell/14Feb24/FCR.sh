#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
X=$2
#ora dobbiamo di nuovo usare shift due volte per eliminare il nome 
#della gerarchia e il numero (dopo averlo salvato) e avere solo i caratteri; 
#NON serve salvare il primo parametro!
shift
shift 

LN= #variabile per calcolare la lunghezza in linee dei file trovati
count=0 #variabile per contare il numero di file trovati
files= #ci serve per raccogliere i nomi dei file trovati

for F in * 
do 
    if test -f $F -a -r $F #se il file esisre ed è leggibile
    then 
         LN=`wc -l < $F` #calcoliamo la lunghezza in linee dei file
         if test $LN -ge $X #se la lunghezza in linee è maggiore o uguale a X
         then 
            count=`expr $count + 1` #incrementiamo il numero di file trovati
            files="$files $F" #inseriamo il file trovato nella nostra lista
        fi 
    fi 
done 

if test "$files" #se abbiamo trovato almeno un file che soddisfa la specifica
then 
    echo directory trovata `pwd`
    for F in $files #per ogni directory trovata
    do 
        #invochiamo la parte C passando come parametri il nome del file trovato e tutte le stringhe passate come parametri
        echo INVOCHIAMO ORA LA PARTE C 
        #main $F $*
    done 
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $X $* #chiamata ricorsiva
    fi 
done 

