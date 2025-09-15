#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char **argv){
    int nread; /*valore ritorno per read*/
    char buffer[BUFSIZ]; /*usato per i caretteri*/
    int fd = 0; /*usato per la open*/

    

    /*se si arriva qui, sono stati passati 0 0 o 1 parametro*/

    if(argc > 1){
        /* tentiamo quindi l'apertura in lettura di questo file*/

        for(int i = 0; i < argc - 1; ++i){
            if((fd = open(argv[i+1], O_RDONLY)) < 0){
                printf("Errore in apertura file %s dato che fd= %d\n", argv[i+1], fd);
                exit(2);
            }
        }


        /*se si arriva qui, fd vale 0 oppure il valore tornato dalla open che ha avuto successo*/

        /*lettura dal file o dallo standard output dei caratteri letti*/
        while((nread = read(fd, buffer, BUFSIZ)) > 0){
            /*scrittura sullo stdout dei caratteri letti*/
            write(1,buffer,nread);

        }
    }

    exit(0);
}