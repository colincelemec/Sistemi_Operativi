#!/bin/sh
#file comandi Cercafile.sh
#ricerca in breadth-first

cd $1  #ci posizioniamo nella directory giusta

if test -f $2 #se si trova il file con il nome giusto
    then
    echo il file $2 si trova in `pwd`
fi

for i in *
do
    if test -d $i -a -x $i
    then
    fi
echo Stiamo per esplorare la directory `pwd`/$i
Cercafile.sh `pwd`/$i $2 #invocazione ricorsiva
done