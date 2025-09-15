#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta

for i in * 
do 
    if test -d $i -a -x $i 
    then 
        #cerchiamo le directory in cui il suo nome sia si 3 caratteri e i cui caratteri dispari siano uguale a C
        case $i in 
        /$2?$2)  #incrementiamo la variabile del conteggio
                #inseriamo il nome assoluto nella variabile temporanea
                echo `pwd`/$i >> $3;;
            *);;
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