#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

/*definizione del TIPO pipe_t come array di 2 interi*/
typedef int pipe_t[2];

int main(int argc, char **argv)
{
    /*---------VARIABILI LOCALI-------------------*/
    int pid; /*pid per fork*/
    int N; /*numero di file passati sulla riga di comando*/
    char Cx; /*carattere passato come ultimo parametro*/
    int fd; /*file descriptor del file aperto da ogni figlio in lettura*/
    long int pos; /*valore per communicare la posizione al padre e quindi all'utente: il valore di pos del primo carattere viene assunto uguale a 1 */

    /*Questo è un schema di communicazione a due PIPE quindi creeremo due pipe: uno dal figlio al padre e l'altro dal padre al figlio*/
    pipe_t *pipedFP; /*array dinamico di pipe per communicazioni figli-padre*/
    pipe_t *pipedPF; /*array dinamico di pipe per communicazioni padre-figli*/
    int i,j; /*per i cicli*/
    char cx; /*variabile che viene usata dal padre per avere all'utemte l'informazione del carattere da sostituire e usata dai figli per recuperare il carattere comunicato al padre*/
    char scarto; /*variabile che viene usata dal padre per eliminare lo '\n' letto dallo stdin*/
    char ch; /*variabile che viene usata dai figli per leggere dal file*/
    int ritorno=0; /*variabile che viene ritornata da ogni figlio al padre e che contiene il numero di caratteri sostituisci nel file*/
    int nr; /*variabile che serve al padre per sapere se non ha letto nulla*/
    int finito; /*variabile che serve al padre per sapere se non ci sono più posizioni da leggere*/
    int status; /*variabile di stato per la wait*/
    /*---------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/

    if(argc < 3)
    {
        printf("Errore sul numero di parametri. Ci vuole almeno 2 !!\n");
        exit(1);
    }

    /*---------CALCOLARE IL VALORE DI N-----------------*/
    N = argc - 2;

    /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/
       /*Controllo che sia un singolo carattere*/
    if(strlen(argv[argc - 1]) != 1)
    {
        printf("Errore: il terzo parametro %s non è un singolo carattere\n", argv[3]);
        exit(2);
    }
    Cx = argv[argc-1][0];

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    /*allocazione dei due array di pipe*/
    pipedFP = (pipe_t *)malloc(N * sizeof(pipe_t));
    pipedFP = (pipe_t *)malloc(N * sizeof(pipe_t));
    if((pipedFP == NULL) || (pipedPF == NULL))
    {
        printf("Errore nella allocazione dinamico per le pipe\n");
        exit(3);
    }

     /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
     for(i=0; i< N; i++)
    {
        if(pipe(pipedFP[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(4);
        }

        if(pipe(pipedPF[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(5);
        }
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processo padre con pid %d con %d processi: ogni figlio deve cercare il carattere %c\n", getpid(), N, Cx);

     /*ciclo di generazione dei figli*/
     for(i=0; i<N ;i++){
        /*CREAZIONE DEI PROCESSI FIGLI*/
        if((pid = fork()) < 0)
        {
            printf("Errore nella fork %d-esima\n", i);
            exit(6);
        }

        if(pid == 0)
        {
            /*codice del figlio Pi*/
            /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL PROCESSO FIGLIO*/
            printf("DEBUG-Sono il processo figlio di indice %d e pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);

            /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
            /*NEL CASO DI N PIPE*/
            for( j=0; j < N; j++)
            {
                close(pipedFP[j][0]);
                close(pipedPF[j][1]);
                if(i != j)
                {
                    close(pipedFP[j][1]);
                    close(pipedPF[j][0]);
                }
            }

            /*ogni figlio apre lo stesso file*/
            if((fd = open(argv[1], O_RDONLY)) < 0)
            {
                printf("Errore nella apertura del file %s\n", argv[i+1]);
                exit(-1);
            }

            /*ognin processo deve leggere dal file associato Fk cercando le occorenze del caratteere Cx*/
            /*leggiamo il file , un carattere all volta , fino alla fine*/
            while(read(fd, &ch, 1))
            {
                /*se troviamo il carattere */
                if(ch == Cx)
                {
                    /*se troviamo il carattere cercato , allora deve communicare al padre
                    la posizione: useremo lseek per tracciare la posizione e partiremo dalla posizione 1*/
                    pos = lseek(fd, 0L, SEEK_CUR);
                    /*communichiamolo al padre*/
                    write(pipedFP[i][1], &pos, sizeof(pos));

                    /*il figlio ora deve aspettare il carattere da sostituire, se '\n' non deve fare nulla*/
                    /*infatti il padre deve communicare al figlio corrispondente o il carattere da sostiruire
                    oppure se può proseguire con la ricerca di altre occorenze di Cx */
                    read(pipedPF[i][0], &cx, 1);
                    printf("DEBUG-FIGLIO, RICEVUTO %c\n", cx);

                    /*adesso il figlio deve controllare che il padre non gli abbia mandato il carattere '\n'*/
                    if(cx != '\n')
                    {
                        /*per poter sovrascrivere il carattere, come richiesto dal padre, bisogna tornare indietro 
                        di una posizione*/
                        lseek(fd,-1L,SEEK_CUR);
                        write(fd,&cx,1);

                        /*il figlio incrementa il valore di ritorno, dato che è stata fatta 
                        una sostituizione*/
                        ritorno++;
                    }
                }
            }
            /*il figlio Pi deve ritornare al padre il valore corrispondente al numero di sostituizioni
            effettuate*/
            exit(ritorno);
        }
     }

     /*codice del padre*/
     /*chiude il lato della pipe che non usa*/

     /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (i=0; i < N ; ++i)
     {
        close(pipedFP[i][1]);
        close(pipedPF[i][1]);
     }

    /*il padre recupera le informazioni dai figli: prima in ordine di posizione
    e poi in ordine di indice*/
    finito=0; /*all'inizio supponiamo che non sia finito nessun figlio*/
    while(!finito)
    {
        finito=1; /*mettiamo finito a 1 perché potrebbero essere finiti tutti i figli*/
        for(i=0; i<N; i++)
        {
            /*si legge la posizione inviata dal figlio i-esimo*/
            nr=read(pipedFP[i][0], &pos, sizeof(pos));
            if(nr != 0)
            {
                finito=0;
                printf("Il figlio di indice %d ha letto dal file %s nella posizione %ld il carattere %c. Se vuoi sostituirlo , fornisci il carattere altrimenti una linea vuota?\n", i, argv[i+1], pos, Cx);
                read(0,&cx,1);
                if(cx != '\n') read(0,&scarto,1);
                /*se è stato letto un carattere, bisogna fare una lettura e vuoto per eliminare il carattere corrispondente all'invio*/
                write(pipedPF[i][1], &cx, 1); /*inviamo comunque al figlio*/
            }
        }
    }

    /*attesa della terminazione dei figli*/
    for (i=0; i < N; i++)
  	{
   		if ((pid = wait(&status)) < 0)
   		{
      			printf("Errore wait\n");
      			exit(7);
   		}
   		if ((status & 0xFF) != 0)
                	printf("Figlio con pid %d terminato in modo anomalo\n", pid);
   		else
   		{
                	ritorno=(int)((status >> 8) & 0xFF);
                    printf("Figlio con pid %d ha ritornato %d\n", pid , ritorno);
   		}
  	} 

    exit(0);
}