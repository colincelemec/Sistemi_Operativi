#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
    int i, n; /*i serve per contare le linee, n per sapere quante linee devono essere mostrate (deriva dall'opzione)*/
    char c; /*per leggere i caratteri da standard input e scriverli su stdout*/
    int fd; /*per il file descriptor*/

    /*controllo numero di parametri*/
    if(argc != 3){
        printf("Errore sul numero di parametri: Necessario 2 argomento");
        exit(1);
    }

    if(argv[1][0] != '-'){
        printf("Errore: l'opzione non è corretta dato che n = %d\n", n);
        exit(2);
    }

    n = atoi(&(argv[1][1])); /*convertiamo il numero che parte dal secondo carattere e quindi escludenso il '-'; nome specificato dal testo*/
    if(n <= 0){
        printf("Errore: l'opzione non è corretta dato che n = %d\n", n);
        exit(3);
    }

    if((fd = open(argv[2], O_RDONLY)) < 0){
        printf("errore nel l'apertura de file %s\n", argv[2]);
        exit(4);
    }

    i=1; /* inizializziamo il conteggio delle linee: la prima linea è la linea numero 1*/
    while(read(fd,&c,1)) /*lettura da stdin*/
    {
        if(c == '\n') i++; /*se troviamo un terminatore di linea, incrementiamo il conteggio delle linee*/
        write(1,&c,1);  /*scriviamo comunque il carattere qualunque sia*/
        if(i>n) break; /*se il conteggio supera n, allora usciamo dal ciclo di lettura*/
    }

    exit(0);
}