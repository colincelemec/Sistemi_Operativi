#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta

conta=0 #variabile che serve per contare i file che soddisfano la specifica
LC= #variabile per contare la lunghezza in caratteri
files= #variabile per salvare i file che soddisfano la specifica

#il nome della directory deve essere di esattamente 3 parametri e i caratteri devono essere dispari

case $1 in 
*/$2?$2) echo DEBUG: Directory che contiene esattmente 3 parametri con i caratteri dispari uguale a $2
            for elem in *
            do 
                if test -f $elem -a -r $elem #se il file esiste ed è leggibile
                then 
                    #calcoliamo la lunghezza in caratteri
                    LC=`wc -c < $elem`
                    if test `expr $LC % 2` -eq 0 -a $LC -ge 1 #se il file a una lunghezza in caratteri pari e non è vuoto
                    then 
                        #incrementiamo conta
                        conta=`expr $conta + 1`
                        files="$files $elem" #salviamo il file trovato dentro la nostra lista
                    fi 
                fi 
            done 
            ;;
    *)  ;;
esac 


if test $conta -ge 1 #se abbiamo trovato almeno un file che soddisfa la specifica
then 
    echo DIRECTORY TROVATA  CON NOME ASSOLUTO CHE RISPETTA LA SPECIFICA: `pwd`
    #si deve invocare la parte C passando come parametri i nomi relativi semplici deil file trovati che soddisfano la specifica
    echo INVOCAZIONE DELLA PARTE CON PARAMETRI: $files
    #main $files
else
    #se non abbiamo trovato nemmeno un file che lo specifica
    echo DIRECTORY TROVATA CHE NON RISPETTA LA SPECIFICA: `pwd`
fi

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2  #chiamata ricorsiva
    fi 
done 

