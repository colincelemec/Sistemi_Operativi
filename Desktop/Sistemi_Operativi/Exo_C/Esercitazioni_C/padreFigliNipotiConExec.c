#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

#define PERM 0644

int main (int argc, char **argv)
{
   int N; 				/* numero di figli */
   int pid;				/* pid per fork */
   int n; 				/* indice per i figli */
   char *FOut;				/* array dinamico di char per il nome del file da creare da parte dei figli */
   int fdw; 				/* per la creat */
   int pidFiglio, status, ritorno;	/* per valore di ritorno figli*/

   /*controllo sul numero di parametri che prevede almeno 3 nomi di file*/
   	if (argc < 4)
	{
		printf("Errore: Necessari almeno 3 parametri per %s e invece argc = %d\n", argv[0], argc);
		exit(1);
	}

    /*individuamo il numero di file e quindi di conseguenza il numero di processi*/
    N = argc-1;
    printf("DEBUG-Sono il processo padre con pid %d e creerò %d processi figli che generanno a loro volta ognuno un nipote\n", getpid(), N);

    /*creazione figli*/
    for(n=0; n < N; n++){
        if((pid=fork()) < 0){
            printf("Errore creazione figlio %d-esimo\n", n);
            exit(2);
        }
        if(pid==0){
            /*codice figlio*/
            printf("DEBUG-Sono il figlio %d di indice %d e sono associato al file %s\n", getpid(), n, argv[n+1]);

            /*i figli devono creare il file specificato COME PRIMA COSA */
            FOut=(char *)(strlen(argv[n+1]) + 6); /*bisogna allocare una stringa lunga come il nome del file + il carattere '.' + i caratteri della parolo sort(4) + il terminatore della stringa*/
            if ((FOut == NULL))
            {
                printf("Errore nelle malloc\n");
                exit(-1);
            }
            /*copiamo il nome del file associato al figlio*/
            strcpy(FOut, argv[n+1]);
            /*concateniamo la stringa specificata dal testo*/
            strcat(FOut, ".sort");
            if((fdw=creat(FOut, PERM)) < 0){
                printf("Impossibile creare il file %s\n", FOut);
                exit(-1);
            }
            /*chiudiamo il file creato dato che comunque il figlio non lo usa*/
            close(fdw);

            /*ogni figlio crea un nipote*/
            if((pid = fork()) < 0)
            {
                printf("Errore nella fork di creazione del nipote\n");
                exit(-1);
            }

            if(pid == 0){
                /*codice nipote*/
                printf("DEBUG-Sono il processo nipote del figlio di indice %d e pid %d sto per eseguire il comando sort per il file %s\n", n, getpid(), argv[n+1]);

                /*chiudiamo lo standard input dato che la specifica dice che dobbiamo usare il cmando-filtro sort*/
                close(0);
                /*apriamo il file associato in sola lettura*/
                if(open(argv[n+1], O_RDONLY) < 0){
                    printf("Errore: FILE %s NON ESISTE o NON LEGGIBILE\n", argv[n+1]);
                    exit(-1);
                }

                /*chiudiamo lo stdout dato che la specifica indica che il risultato del comando-filtro sort va scritto sul file creato da ogni figlio(padre di questo processo)*/
                close(1);
                /*apriamo il file creato in sola scrittura*/
                if(open(FOut, O_WRONLY) < 0){
                    printf("Errore: FILE %s NON si riesce ad aprire in scrittura\n", FOut);
                    exit(-1);
                }

                /*il nipote diventa il comando-filtro sort: bisogna usare le versioni dell'exec con la p in fondo in modo da usare la variabile di ambiente PATH: NON serve alcun parametro*/
                execlp("sort", "sort", (char *)0);
                /*usiamo perror che scrive su stderr, dato che lo stdout è collegato ad un file*/
                perror("Problemi di esecuzione del sort da parte del nipote");
                exit(-1);
            }/*qui termina il codice del nipote*/
            /*riprendiamo il codice del figlio*/
            /*il figlio deve aspettare il nipote per ritornare, a sua volta, al padre il valore tornato dal nipote*/
            if((pidFiglio=wait(&status)) < 0){
                printf("Errore nella wait\n");
                exit(-1);
            }
            if((status & 0xFF) != 0){
                printf("Nipote con pud %d terminato in modo anomalo\n", pidFiglio);
                exit(-1);
            }
            ritorno=(int)((status >> 8) & 0xFF);
            /*il figlio ritorna il valore ricevuto dal nipote*/
            exit(ritorno);
             
        } /*qui termina codice del figlio*/
    } /*fine for*/

    /*codice del padre*/
    /*il padre aspetta i figli*/
    for(n=0; n < N; n++){
        if((pidFiglio=wait(&status)) < 0){
            printf("Errore in wait\n");
            exit(3);
        }
        if((status & 0xFF) != 0)
            printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
        {
            ritorno=(int)((status >> 8) & 0xFF);
            printf("Il figlio con pid=%d ha ritornato %d\n", pidFiglio, ritorno);
        }
    }

    exit(0);

}
