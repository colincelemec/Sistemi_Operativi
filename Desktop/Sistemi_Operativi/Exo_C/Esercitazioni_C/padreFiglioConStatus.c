#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> /*per la wait*/
#include <time.h> /*per la inizializzazione del seme per la rand*/

int mia_random(int n){
    int casuale;
    casuale = rand() % n;
    return casuale;
}

int main(){
    int pid; /*per il valore di ritorno della fork*/
    int pidFiglio; /*per valore di ritorno della wait*/
    int status; /*per usarlo nella wait*/
    int ritorno; /*per filtrare valore di uscita del figlio*/

    /*stampiamo quanto richiesto dal testo*/
    printf("sono il processo padre con pid %d\n", getpid());
    srand(time(NULL)); /*inizializziamo il seme per la generazione random dei numeri*/

    if((pid = fork()) < 0){
        printf("ERRORE NELLA FORK\n");
        exit(1);
    }

    if(pid == 0){
        /*figlio*/
        int r; /*per valore generato random*/

        /*stampiamo quanto richiesto dal testo*/
        printf("sono il processo figlio con pid %d e sono stato generato dal processo padre con pid %d\n", getpid(), getppid());
        r=mia_random(100);
        printf("DEBUG-sono il figlio e sto per ritornare al padre il valore %d\n", r);
        /*il figlio deve tornare il valore random calcolato che sicuramente è minore di 255*/
        exit(r);
    }

    /*padre*/
    /*il padre aspetta il figlio*/
    if((pidFiglio=wait(&status)) < 0){
        printf("ERRORE NELLA WAIT %d\n", pidFiglio);
        exit(2);
    }

    if((status & 0xFF) != 0){
        printf("figlio terminato in modo involontario\n");
    }else{
        ritorno=(int)((status >> 8) & 0xFF);
        printf("il figlio con pid=%d ha ritornato %d\n", pidFiglio,ritorno);
    }

    exit(0);

    /*
    Nel caso dove si deve fare padreSenzaFigli.c basta non generare il figlio cioè
    non invocare la fork() per la creazione di un processo figlio !!
    */
}




