#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
cont=0 #per contare i numero di file che si trovano dentro la directory
LN= #variabile che serve a calcolare la lunghezza in linee del file trovato
files= #variabile che serve a stockare i file trovati

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
            case $F in #verfichiamo che il suo nome contiene esattamente due caratteri
            ??) 
                LN=`wc -l < $F`
                if test $LN -eq $3 
                then 
                    echo Abbimao trovato il file $F
                    files="$files $F"
                    cont=`expr $cont + 1`
                fi ;;
            *) ;;
            esac     
    fi 
done 

if test $cont -lt $2 -a $cont -ge 2 #se il numero di file presente nella directory è < di M e >= 2
then 
    echo directory trovata `pwd`
    echo invocazione della parte C
    main $files 
fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2 $3 #chiamata ricorsiva
    fi 
done 


