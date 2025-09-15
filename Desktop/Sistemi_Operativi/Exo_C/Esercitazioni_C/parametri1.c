#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    int n; /*n serve per scorrere i parametri*/
    int N; /*N serve per salvare il numero dei parametri*/

    /*Controllo numero di parametri*/
    if(argc < 2){ 
        printf("Errore: Necessario almeno 1 parametro per %s e invece argc = %d\n", argv[0], argc);
        exit(1);
    }

    /*Salviamo, come richiesto il numero di parametri nella variabile di nome N*/
    N = argc - 1; /*Ricordarsi che argc conta sempre anche il nome dell'eseguibile*/
    printf("Eseguo il programma %s con %d parametri\n", argv[0], N);
    for(n = 0; n < N; ++n){
        printf("Il parametro di indice %d è %s\n", n+1, argv[n+1]);
    }

    exit(0);

}