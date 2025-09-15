#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>


int main(int argc, char **argv){

    int i,n; /*i serve per contare le linee , n intero corrispondente al secondo parametro*/
    int j; /*indice per il buffer che contiene la linea corrente*/
    char buffer[255]; /*per leggere i caratteri dal file, supponendo che una linea son sia più lunga compreso il terminatore di linea e il terminatore di stringa perché trasformeremo la linea in una stringa per poterla stampare con printf*/
    int fd; /*per la open*/
    int trovata = 0; /*indica se è stata trovata la linea n-esima: usata come un booleano*/

    /*controllo numero di parametri*/
    if(argc != 3){
        printf("Errore: Necessari 2 argomenti (nome file e lunghezza linea) per %s, mentre argc = %d\n", argv[0],argc);
        exit(1);
    }

    /*controllo primo parametro: apriamo il file in lettura*/
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore: FILE %s NON ESISTE o non leggibile\n", argv[1]);
        exit(2);
    }

    /*controllo secondo parametro*/
    n = atoi(argv[2]); /*convertiamo il secondo parametro*/
    if ( n <= 0){
        printf("Errore: il secondo parametro %s non è un numero strettamente positivo\n", argv[2]);
        exit(3);
    }

    i=1; /*inizializziamo il conteggio delle linee: la prima linea è la linea numero 1*/
    j=0; /*valore iniziale dell'indice del buffer*/
    while(read(fd, &(buffer[j]), 1) != 0){ /*leggiamo un carattere alla volta e lo inseriamo nell'array buffer*/
        if(buffer[j] == '\n'){ /*se il carattere corrente è il terminatore della linea*/
            if(n == j+1) /*controlliamo se la lunghezza della linea corrente ha la lunghezza giusta e quindi la stampiamo dopo averla resa una stringa. N.B: in questo caso non va inserito una break perché potremo chiaramente trovare piu linee della lunghezza cercata*/
            {
                buffer[j+1] = '\0'; /*dobbiamo prima rendere la linea una stringa*/
                printf("la linea numero %d del file %s ha la lunghezza cercata cioè %d:\n%s", i, argv[1], n, buffer);
                trovata=1; /*setttiamo la variabile , dato che abbiamo trovata almeno una linea delle lunghezza cercata*/
            }
            j = 0; /*azzeriamo l'indice per la prossima linea*/
            i++; /*e incrementiamo il conteggio delle linee*/
        }
        else j++; /*altrimenti incrementiamo l'indice nel buffer*/
    }

    if(!trovata){
        printf("Non è stata trovata alcuna linea nel file %s di linghezza %d\n", argv[1], n);
    }
    exit(0);
}