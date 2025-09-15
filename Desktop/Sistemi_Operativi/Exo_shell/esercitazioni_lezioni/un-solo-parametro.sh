
#!/bin/sh

#controllo sul numero di parametri
if test $# -ne 1
then
    echo Errore sul numero di parametri passati
    exit 1
fi

#controllo se è un nome assoluto, relativo o relativo semplice

case $1 in
/*) echo " $1 è un nome assoluto";; #solo se inizia col carattere / è un nome assoluto
*/*) echo " $1 è un nome relativo";; #solo se contiene il carattere / allora è un nome relativo
*) echo "$1 è un nome relativo semplice";;
esac 

