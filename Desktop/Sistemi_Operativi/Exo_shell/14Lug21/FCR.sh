#!/bin/sh 
#FCR.sh

cd $1 #ci spostiamo nella directory giusta

#la variabile L ci serve per contare il numero di linee
L=

#la variabile cont ci serve per contare il numero di file, nome indicato nel testo
cont=0

#la variabile files ci serve per memorizzare i file trovati: questa variabile viene usata come una lista
files=

for F in * #per ogni elemento della directory corrente , F nome indicato nel testo
do 
    #controlliami solo i nomi dei file , come indicato ella specifica
    if test -f $F # se il file esiste
    then 
        #controlliamo la lunghezza del file
        case $F in 
        ??) #il nome deve contenere esattamente 2 caratteri
            #calcoliamo la lunghezza in linee del file per vedere se soddisfa le specifiche
            L=`wc -l < $F`
            if test $L -eq $3 
            then 
                #aggiorniamo la variabile del conteggio
                cont=`expr $cont + 1`
                #salviamo il nome del file nella variabile apposita 
                files="$files $F"
            fi ;;
        *) #non si deve dare nulla
        esac
    fi 
done 


if test $cont -lt $2 -a $cont -ge 2 #se il numero di file trovati è strettamente minore di H e maggiore di 2
then 
    echo TROVATO directory `pwd` 
    #dobbiamo invocare la parte C passando i nomi dei file precedentemente salvati insieme con il valore M
    echo invocazione della parte C
    #main $files $3
fi 

#chiamata ricorsiva

for i in *
do 
    if test -d $i -a -x $i 
        then 
        #chiamata ricorsiva cui passiamo il nome assoluto della dir
        $0 `pwd`/$i $2 $3
    fi 
done

