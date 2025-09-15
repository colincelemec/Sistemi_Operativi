#!/bin/sh
#file ricorsivo FCR.sh

cd $1  #ci posizioniamo nella directory giusta
LN= #variabile per calcolare la lunghezza in linee del file
N= #per il numero di righe che contengono almeno un carattere numerico
count=0 #per contare i file trovati
files= #per salvare tutti i file trovati

for F in * 
do 
    if test -f $F -a -r $F #se il file esiste ed è leggibile
    then 
         #SCRIVERE IL CODICE DELLA SPECIFICA
         #calcoliamo la lunghezza in linee del file trovato
            LN=`wc -l < $F`
         #tutte le sue linee devono contenere almeno un carattere numerico
         #calcolimao il numerio di righe che contengon almeno un carattere numerico
         N=`grep '[0-9]' $F | wc -l`
         if test $LN -eq $2 -a $N -eq $LN #se il numero di righe richiesto è uguale a X e se in tutte le sue linee c'è almeno un carattere numerico
         then 
                count=`expr $count + 1` #incrementiamo count
                files="$files $F" #mettiamo dentro la nostra lista il file trovato
        fi 
         
    fi 
done 

if test $count -ge 1 #se abbiamo trovato almeno 1 file che soddisfa la specifica
then 
    echo DIRECTORY TROVATA `pwd`
    #chiediamo la conferma all'utente se si deve invocare la parte C o no
    echo  "Vuoi invocare la parte C (y/s) ? :  "
    read answer 
    case $answer in 
    y) echo INVOCAZIONE DELLA PARTE C PASSANDO COME PARAMETRI I FILE trovati CHE SONO : $files
        main $files;;
    *) echo NON INVOCAZIONE DELLA PARTE C;;
    esac 

fi 

#INVOCAZIONE RICORSIVA
for i in *
do 
    if test -d $i -a -x $i 
    then 
        FCR.sh `pwd`/$i $2  #chiamata ricorsiva
    fi 
done 


