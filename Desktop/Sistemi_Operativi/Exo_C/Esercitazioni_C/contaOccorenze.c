#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv){
    char c ; /*per leggere i caratteri dal file*/
    /*Controllo sul numero di parametri*/
     if(argc != 3){
        printf("Errore: Necessitano esattamante 2 parametri per %s e invece argc = %d\n", argv[0], argc);
        exit(1);
     }

     /*Controllo sul tipo di parametri*/
     /*Il primo parametro deve essere il nome di un file*/
     int fd;
     char *F=argv[1];
     if((fd = open(F, O_RDONLY)) < 0){
        printf("Errore: impossibile di aprire il file %s\n", F);
        exit(2);
     }

     /*il secondo parametro deve essere un singolo carettere*/
     
     if(strlen(argv[2]) != 1){
        printf("Errore: il terzo parametro %s non è un singolo parametro\n", F);
        exit(3);
     }

     char Cx = argv[2][0];
    /*Visualizziamo sullo stdout il nome dell'esguibile e il valordei due parametri*/
    printf("eseguo il programma %s con %d parametri\n", argv[0], argc);
    printf("Il primo parametro è il file con nome %s e il secondo è il singolo carattere %c\n", F, Cx);

    /*Si calcoli quanti occorenze di Cx si trovano nel file F*/
    long int totale = 0; /*per contare il numero di occorenze*/

    while(read(fd, &c, 1)){
        if(c == Cx){
            ++totale;
        }
    }
    
    printf("il numero di occorenze del carattere %c nel file %s è %ld\n", Cx, F, totale);
    exit(0);
}