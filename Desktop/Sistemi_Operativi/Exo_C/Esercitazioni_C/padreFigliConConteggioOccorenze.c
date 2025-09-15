#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    char Cx; /*carattere che i figli devono cercare nel file a loro associato*/
    int N; /*numero di figli*/
    int pid; /*pid per fork*/
    int n; /*indice per i figli*/
    int totale=0; /*serve per calcolare il numero di occorenze*/
    int fd; /*per la open*/
    char c; /*per leggere i caratteri da ogni file*/
    int pidfiglio, status, ritorno; /*per valore di ritorno figli*/

    /*controllo sul numero di parametri*/
    if( argc < 4){
        printf("Errore: Necessari almeno 3 parametri per %s e invece argc = %d\n", argv[0], argc);
        exit(1);
    }

    if(strlen(argv[argc-1]) != 1){
        printf("Errore ultimo parametro non singolo carattere dato che è %s\n", argv[argc-1]);
        exit(2);
    }

    /*individuamo il carattere da cercare*/
    Cx= argv[argc-1][0];

    /*individuamo il numero di file e quindi di conseguenza il numero di processi*/
    N=argc-2; /*si deve togliere il nome dell'eseguibile anziché l'ultimo parametro*/

    printf("DEBUG-sono il processo padre con pid %d e creerò %d processi figli che cercheremo il carattere %c nei file passati come parametri\n", getpid(), N, Cx);

    /*creazione figli*/
    for(n=0; n < N ; ++n){

        if((pid=fork()) < 0){
            printf("Errore creazione figlio %d-esimo\n",n);
            exit(3);
        }

        if(pid == 0){
            /*codice figlio*/
            printf("DEBUF-sono il figlio %d di indice %d e sono associato al file %s\n", getpid(), n, argv[n+1]);
            /*apriamo il file*/
            if((fd = open(argv[n+1], O_RDONLY)) < 0){
                printf("Errore: FILE %s NON ESISTE o NON LEGIBILE\n", argv[n+1]);
                exit(-1);
            }
            /*leggiamo il file*/
            while(read(fd,&c,1) != 0){
                if(c==Cx) totale++; /*se troviamo il carattere incrementiamo il conteggio*/
            }
            /*ogni figlio deve tornare il numero di occorenze e quindi totale*/
            exit(totale);
        }
    } /*fine for*/

    /*codice del padre*/
    /*il padre aspetta i figli*/
    for(n=0; n < N; n++){
        if((pidfiglio=wait(&status)) < 0){
            printf("Errore nella wait\n");
            exit(4);
        }
        if((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidfiglio);
        else{
            ritorno=(int)((status >> 8) & 0xFF);
            printf("il figlio con pid=%d ha ritornato %d\n", pidfiglio,ritorno);
        }
    }
    exit(0);
}