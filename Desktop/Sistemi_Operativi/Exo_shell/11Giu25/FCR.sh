#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
count=0 #per contare le occorenze del carattere 

for F in *
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
        #i file devono contenere almeno una occorenza di tutti i caratteri C 
        if test "grep '$2' $F" #se trovo una occorenza di tutti i caratteri C nel file F
        then 
            count=`expr $count + 1` #incremento il conteggio
            echo $F >> $3 #inserisco il file trovato nel file temporaneo
        fi 
    fi 
done 

#per ogni directory analizzata, si deve ritornare il nome assoluto insieme con l'indicazione se è stato trovato almeno un file che soddisfa la specifica

if test $count -ge 1
then 
    echo directory trovata `pwd`
    echo è stato trovato almeno un file che soddisfa la specifica
else
    echo Non è stato trovato un file che soddisfa la specifica
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 