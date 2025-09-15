#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
LC= #variabile che serve a calcolare la lunghezza in caratteri del file trovato

for i in * 
do 
    if test -f $i -a -w $i -a -r $i  #se il file esiste ed è leggibile e scrivibile
    then 
        #verifichiamo se ha il nome relativo S.txt
        case $i in 
        $2.txt) #salviamo la lunghezza in caratteri e il nome assoluto del file trovato
                #calcoliamo la lunghezza in caratteri
                LC=`wc -c < $i`
                echo il nome assoluto del file trovato è `pwd`/$i #nome assoluto del file trovato
                echo $LC `pwd`/$i >> $3 #salviamo il file trovato nel nostro file temporaneo;; 
        esac
    fi 
done 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 
            
