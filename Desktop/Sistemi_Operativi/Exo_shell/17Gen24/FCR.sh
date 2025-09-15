#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta

count=0 #per contare le directory che soddisfano la specifica
files= #per stockare gli file che soddisfano la specifica
LN= #per calcolare la la lunghezza in file del file trovato 
conta=`expr $3 + 1` #il primo livello verrà contato come livello 1

#verifichiamo che la directory si trova ad un livello pari
if test `expr $conta % 2` -eq 0 #se si trova in un livello pari
then 
    #incrementiamo count
    count=`expr $count + 1`
    echo DIRECTORY TROVATO `pwd`

    for F in * 
    do 
        if test -f $F -a -r $F #se il file esiste ed è leggibile
        then 
            #calcoliamo la lunghezza in linee delmfile trovato
            LN=`wc -l < $F`
            if test $LN -eq $2
            then 
                #scriviamo dentro la nostra lista il file trovato
                files="$files $F"
            fi 
        fi 
    done 

    if test "$files" #se abbiamo trovato almeno un file
    then 
        #invochiamo la parte C
        echo INVOCAZIONE DELLA PARTE C
        #main $files 
    fi 
fi 



#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 