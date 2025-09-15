#!/bin/sh
#File comandi ricorsivo dirass X  


cd $1 #ci spostiamo nella directory giusta

#definiamo una variabile NR per contenere il numero di righe
NR= 

#definiamo una variabile N per contenere il numero di righe che contengono almeno un carattere numerico

N=

#definiamo una variabile files per memorizzare i nomi dei file; questa variabile viene usata come lista
files=

for i in * #per ogni elemento della directory corrente
do 
    if test -f $i -a -r $i #se è un file ed è leggibile
    then 
        #calcoliamo il numero di righe
        NR=`wc -l < $i`
        #calcoliamo quante linee contengono almeno un carettere numerico
        N=`grep '[0-9]' $i | wc -l`
        #echo NR is $NR e N is $N
        if test $NR -eq $2 -a $N -eq $NR #se il numero di linee è quello richiesto e se in tutte le linee c'è almeno un carattere numerico
        then 
            files="$files $i" #salviamo il nome del file
        fi 
    fi 
done 

#se abbiamo trovato almeno un file
if test "$files"
then 
    echo TROVATO DIRECTORY `pwd`
    echo che contiene i seguenti file che soddisfano la specifica $files 
    echo DEVO CHIAMARE LA PARTE C?
    read risposta
    case $risposta in 
    si) echo Invochiamo la parte C;;
    *) echo Nessuna invocazione della parte c;;
    esac
fi 


for i in *
do 
    if test -d $i -a -x $i 
    then 
        #invocazione ricorsiva
        FCR.sh `pwd/$i` $2 
    fi 
done 