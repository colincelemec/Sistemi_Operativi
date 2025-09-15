#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
LN= #serve per calcolare la lunghezza in linee del file


for f in * 
do 
    if test -f $f -a -r $f #se il file esiste ed è leggibile
    then 
        #calcoliamo lal lunghezza in linee del file
        LN=`wc -l < $f`
        if test $LN -ge $2 
        then 
            if test $LN -ge 5 #controllimao la lunghezza in linee del file rispetto a 5
            then 
                #si seleziona la singola linea numero 5 a partire dell'inizio del file e 
                #scriviamo questa linea in un file appositamente creato avento nome f.quinta

                head -5 $f | tail -1 > $f.quinta 
                echo `pwd`/$f.quinta >> $3 #inseriamo nel nostro file temporaneo il file trovato
               
            else 
            #se il file è più corto e non sia possibile selezionare la linea 
            #creaimo un file vuoto di nome f.NOquinta
            > $f.NOquinta
            echo `pwd`/$f.NOquinta >> $3
            fi
        fi 
    fi 
done 


#chiamata ricorsiva

for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #invocazione ricorsiva
    fi 
done 



