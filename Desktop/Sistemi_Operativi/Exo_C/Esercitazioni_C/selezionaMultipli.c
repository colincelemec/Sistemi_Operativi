#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
    int nr; /*per la read*/
    int i; /*indice del multiplo*/
    char *buffer; /*buffer DINAMICO per leggere i caratteri dal file*/
    
    /*controllo sul numero di parametri*/
    if(argc != 3){
        printf("Errore sul numero di parametri passati. Ci vuole esattamente 2 parametri!");
        exit(1);
    }

    /*controllo sul tipp di parametri*/
    /*Il primo parametro deve essere il nome di un file*/

    int fd;
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore nel l'apertura del file %s\n", argv[1]);
        exit(2);
    }
    int n = atoi(argv[2]);
    /*Controlliamo che n sia un numero strettamente positivo*/
    if(n <= 0){
        printf("il secondo parametro non è un numero strettamente positivo!\n");
        exit(3);
    }

    /*arrivato qui, è finito il controllo sul numero e sul tipo di parametri*/

    /*allochiamo la memoria necessaria per il buffer sull base del valore di n*/
    buffer = (char *)(malloc(n * sizeof(char)));
    if(buffer == NULL){
        printf("Errore: problemi nella malloc\n");
        exit(4);
    }

    printf("DEBUG-Stiamo per selezionare i caratteri multipli di %d\n", n);
    i = 1;
    while((nr = read(fd, buffer, n)) > 0){
        if(nr == n){
            /*possiamo selezionare l'n-esimo carattere*/
            printf("Il carattere multiplo %d-esimo all'interno del file %s è %c\n", i, argv[1], buffer[n-1]);
            i++;
        }else printf("il file non ha dimensione multipla di %d\n", n);
    }

    
    exit(0);


}