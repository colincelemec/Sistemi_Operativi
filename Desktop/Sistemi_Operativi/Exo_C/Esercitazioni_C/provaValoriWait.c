#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    int pid; /*per il valore di ritorno della fork*/
    int pidFiglio; /*per valore di ritorno della wait*/
    int status; /*per usarlo nella wait*/
    int ritorno; /*per filtrare valore di uscita del figlio*/
    int valore; /*valore per scanf da parte de figlio*/

    if((pid = fork()) < 0){
        /*fork fallita*/
        printf("Errore in fork\n");
        exit(1);
    }

    if(pid == 0){
        /*figlio*/
        printf("DEBUG-Esecuzione del figlio\n");
        /*questa volta facciamo fare qualche cosa al figlio*/
        printf("Dammi un valore intero per provare la exit: \n");
        scanf("%d", &valore);
        if((valore > 255) || (valore < 0)){
            printf("ATTENZIONE IL VALORE SARA TRONCATO!\n");
        }else{
            printf("il valore fornito non verrà troncato!\n");
        }

        exit(valore);
    }

    /*padre*/
    printf("DEBUG-generato figlio con PID = %d\n", pid);

    /*il padre aspetta il figlio*/
    if((pidFiglio=wait(&status)) < 0){
        printf("Errore in wait\n");
        exit(2);
    }

    if((status & 0xFF) != 0)
        printf("Figlio terminato in modo involontario\n");
    else{
        /*selezione del byte "alto"*/
        ritorno=(int)((status >> 8) & 0xFF);
        printf("il figlio %d ha ritotnato il valore intero %d\n", pidFiglio, ritorno);
    }

    exit(0);
}