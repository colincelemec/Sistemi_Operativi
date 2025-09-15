#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
    int fd; /*per la open*/
    int N; /*serve per convertire il secondo parametro*/
    char C; /*serve per selezionare il terzo parametro*/

    /*Controllo numero di parametri*/
    if(argc != 4){
        printf("Errore: Necessitano esattamante 3 parametri per %s e invece argc = %d\n", argv[0], argc);
        exit(1);
    }

    /*controllo primo parametro: dato che viene detto che deve essere considerato il nome di un file andiamo a aprovare ad aprirlo*/
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore: File %s NON ESISTE\n", argv[1]);
        exit(2);
    }

    /*Controllo secondo parametro: usiamo la funzione di libreria atoi*/
    N=atoi(argv[2]);
    if(N <= 0){
        printf("Errore; il secondo parametro %s non è un numero strettamente maggiore di 0\n", argv[2]);
        exit(3);
    }

    /*Controllo terzo parametro*/
    if(strlen(argv[3]) != 1)
    {
        printf("Errore: il terzo parametro %s non è un singolo carattere\n", argv[3]);
        exit(4);
    }

    /*Isoliamo il singolo carattere*/
    C = argv[3][0];

    /*Controlli sui parametri, in numero e in tipo ,FINITI*/
    printf("Eseguo il programma %s con %d parametri\n", argv[0], argc - 1);
    printf("Il primo parametro è il nome di un file %s\nIl secondo parametro è il numero strettamente positivo %d\nE il terzo parametro è il singolo carattere %c\n", argv[1], N, C);
    exit(0);
}