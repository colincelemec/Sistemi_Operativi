#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #variabile per conteggiare il file trovato

for i in * 
do 
    if test -f $F -a -r $F
    then 
         case $F in 
         *.$2) 
                #il file ha la terminazione .S
                #incrementiamo la variabile di conteggio
                count=`expr $count + 1`
                ;;
            *);;
        esac

    fi 
done 

if test $count -ge 1
then 
    #contiene almeno un file che soddisfa i requisiti
    echo directory trovata `pwd`
    #inseriamo lo nel nostro file temporaneo
    echo `pwd` >> $3
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 

