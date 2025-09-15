#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv){
    
    long int newpos;
    char change;
    int fd;
    char c;
    
    /*controllo sul numero di parametri*/
    if(argc != 4){
        printf("Errore sul numero di parametri passati. Ci vuole esattamente 3 parametri!");
        exit(1);
    }

    /*controllo sul tipp di parametri*/
    /*Il primo parametro deve essere il nome di un file*/

    
    if((fd = open(argv[1], O_RDWR)) < 0){
        printf("Errore nel l'apertura del file %s\n", argv[1]);
        exit(2);
    }

    /*il secondo parametro deve essere un singolo carattere */

    if(strlen(argv[2]) != 1){
        printf("Errore: il secondo parametro non è un singolo carattere");
        exit(3);
    }

    char Cx = argv[2][0];

    if(strlen(argv[3]) != 1){
        printf("Errore: il terzo parametro non è un singolo carattere");
        exit(4);
    }
     change = argv[3][0];

  
    while(read(fd, &c, 1)){

        if( c == Cx){
            //bisogna portare il file indietro di 1 con la primitiva lseek
            newpos=lseek(fd, -1L , 1);

            write(fd, &change, 1);
        }
    }

exit(0);

}