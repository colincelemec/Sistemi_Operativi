#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
LC= #variabile per calcolare la lunghezza in caratteri del file trovato
files_pari= #variabile per stockare i file in posizione pari trovati
files_dispari= #variabile per stockare i file in posizione dispari trovati
count=0 #per contare i file trovati

for F in * 
do 
    if test -f $F  #se il file esiste 
    then 
         #SCRIVERE IL CODICE DELLA SPECIFICA
         #calcoliamo la lunghezza in caratteri del file trovato
         LC=`wc -c < $F`
         if test $LC -eq $3 #se la lunghezza in caratteri è uguale a K
         then 
              count=`expr $count + 1` #incrementiamo count
              if test `expr $count % 2` eq 0
              then 
                files_pari="$files_pari $F" #mettiamo dentro la nostra lista il file trovato
              else 
                files_dispari="$files_dispari $F" #mettiamo dentro la nostra lista il file trovato
              fi 
         fi 
         
    fi 
done 

if test $count -eq $2 #se il numero di files trovati è uguale a H
then 
    echo DIRECTORY TROVATA
    echo INVOCAZIONE DELLA PARTE C con file in posizione pari
    main $files_pari
    echo INVOVAZIONE DELLA PARTE C con file in posizione dispari
    main $files_dispari 
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 


