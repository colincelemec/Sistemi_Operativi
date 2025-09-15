#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #variabile per contare i file trovati

for F in * 
do 
    if test -f $F -a ! -x $F #se il file esiste ed a come estensione .S
    then 
         #SCRIVERE IL CODICE DELLA SPECIFICA
         #il nome del file deve avere come estensione .S e deve essere NON eseguibile
         case $F in 
         *.$2)  
                count=`expr $count + 1` #incrementiamo count  
                echo `pwd`/$F >> $3
                ;;
            *)  ;;
         esac
         
    fi 
done 

if test $count -ge 1 #se abbiamo trovato almeno 1 file
then 
    echo DIRECTORY TROVATA `pwd`
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 
