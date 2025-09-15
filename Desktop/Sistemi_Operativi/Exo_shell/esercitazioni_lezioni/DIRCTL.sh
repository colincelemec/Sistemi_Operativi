#!/bin/sh
#file comandi DIR.sh

#controllo sul numero di parametri

case $# in
0) echo non sono stati passati parametri
echo DEBUG-eseguo 'ls -l' paginata nella directory corrente
ls -l | more
exit 0;;
1) echo passato un parametro $1
   echo `ls -l $1`;;
*) echo Errore: Voglio o zero oppure un solo parametro, mentre ne hai passato $#
esac



 



