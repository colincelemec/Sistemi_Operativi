#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
    int i , n; /*i serve per contare le linee, n intero corrispondente al secondo parametro*/
    int j; /*indice per il buffer che contiene la linea corrente*/
    char buffer[255]; /*per leggere i caratteri dal file, supponendo che una linea non sia più lunga di 255 compreso il terminatore di linea ed il terminatore di stringa perché trasformeremo la linea in una stringa per poterla stampare con printf*/
    int fd; /*per la open*/
    int trovata=0; /*indica se è stata trovata la linea n-esima: usata come un booleano*/

    /*controllo numero di parametri*/
    if(argc != 3){
        printf("Errore: necessari 2 argomenti (nome file e numero di linea) per %s, mentre %s, mentre argc=%d\n", argv[0], argc);
        exit(1);
    }

    /*controllo primo parametro: apriamo il file in lettura*/
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore: File %s non esiste o non leggibile\n", argv[1]);
        exit(2);
    }

    /*controllo secondo parametro*/
    n = atoi(argv[2]); /*convertiamo il secondo parametro*/
    if (n <= 0){
        printf("Errore: Il secondo parametro %s non è un numero strettamente positivo\n", argv[2]);
        exit(3);
    }

    i = 1; /*inizializziamo il conteggio delle linee: la prima linea è la linea numero 1*/
    j = 0; /*valore iniziale dell'indice del buffer*/
    while(read(fd, &(buffer[j]), 1) != 0){ /*leggiamo un carattere alla volta e, contestualmente , lo inseriamo nell'aray buffer*/
        if(buffer[j] == '\n'){ /*se il carattere corrente è il terminatore di linea...*/
            if( n == i){ /*controlliamo se siamo sulla linea che dobbiamo selezionare*/
                /*dobbiamo prima rendere la linea una stringa*/
                buffer[j+1] = '\0';
                /*poi la stampiamo su stdout*/
                printf("la linea numero %d del file %s è: \n%s", n, argv[1], buffer);
                trovata=1; /*settiamo la variabile, dato che abbiamo trovato la linea cercata*/
                break; /*usciamo dal ciclo di lettura*/
            }else{ /*se non siamo sulla linea giusta*/
                j = 0; /*azzeriamo l'indice per la prossima linea*/
                i++; /*incrementiamo il conteggio delle linee*/
            }
        } 
        else j++; /*altrimenti incrementiamo l'indice nel buffer perché vuole dire che non siamo alla fine della linea*/
    }
    if(!trovata){ /*se non abbiamo trovato la linea richiesta*/
        printf("la linea numero %d non esiste in %s\n", n, argv[1]);
    }

    exit(0);
    
}