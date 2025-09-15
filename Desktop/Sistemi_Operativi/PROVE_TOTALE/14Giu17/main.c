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
    int fd; /*variabile per la open*/
    int i,j; /*per i cicli*/
    int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
    long int pos; /*per calcolare la posizione nella pipe figli-padre*/
    pipe_t *pipedfp; /*array dinamico di pipe per communicazioni figli-padre*/
    pipe_t *pipedpf; /*array dinamico di pipe per communicazioni padre-figli*/
    char c; /*per leggere dal file*/
    char Cx; /*variabile per l'ultimo parametro*/
    char cx; /*per la posizione nella pipe padre-figli*/
    char scarto; /*variabile usata dal padre per eliminare il '\n'*/
    int pidFiglio, status; /*variabili per wait*/
    int ritorno = 0; /*inizializziamo la variabile di ritorno*/
     /*-----------------------------------------------*/

     /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
     if(argc < 3)
     {
        printf("Errore sul numero di parametri. Ci vuole almeno 2 parametri\n");
        exit(1);
     }

     /*---------CALCOLARE IL VALORE DI N-----------------*/
     N = argc - 2; /*si toglie il nome dell'eseguibile e l'ultimo parametro*/

     /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/
       /*Controllo che sia un singolo carattere*/
    if(strlen(argv[argc - 1]) != 1)
    {
        printf("Errore: l'ultimo parametro %s non è un singolo carattere\n", argv[3]);
        exit(2);
    }
    /*Isoliamo il singolo carattere*/
    Cx = argv[argc - 1][0];

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    pipedfp = (pipe_t *)malloc(N * sizeof(pipe_t));
    pipedpf = (pipe_t *)malloc(N * sizeof(pipe_t));

    if(pipedfp == NULL)
    {
        printf("Errore nella allocazione dinamico per la pipe figli-padre\n");
        exit(3);
    }

    if(pipedpf == NULL)
    {
        printf("Errore nella allocazione dinamico per la pipe padre-figli");
        exit(4);
    }

    /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
    for(i=0; i< N; i++)
    {
        if(pipe(pipedfp[i]) < 0 || pipe(pipedpf[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(5);
        }
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N);

    /*ciclo di generazione dei figli*/
     for(i=0; i<N ;i++) /*inizio ciclo for*/
     {
        /*CREAZIONE DEI PROCESSI FIGLI*/
       if((pid = fork()) < 0)
       {
         printf("Errore nella fork di indice %d-esima\n", i);
         exit(6);
       }

       if(pid == 0)
       {
          /*codice del figlio Pi*/
        /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL PROCESSO FIGLIO*/
        printf("DEBUG-Sono il processo figlio con pid %d e sono associato al file %s e sto per cercare il carattere %c\n", getpid(), argv[i+1], Cx);

        /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
        for( j=0; j<N; j++)
        {
            close(pipedfp[j][0]);
            close(pipedpf[j][1]);
            if(j!=i)
            {
            close(pipedfp[j][1]);
            close(pipedpf[j][0]);
            }
        }

        /*ogni figlio apre lo stesso file*/
        if((fd = open(argv[i+1], O_RDONLY)) < 0)
        {
            printf("Errore nella apertura del file %s\n", argv[i+1]);
            exit(-1);
        }

        /*ogni processo figlio deve leggere i caratteri da file cercando le occorenze del carattere Cx */
        while(read(fd, &c,1))
        {
            if(c == Cx) /*se abbiamo trovato un occorenza */
            {
                
                /*deve communicare al padre la posizione di tale occorenza a partire dall'inizio del file*/
                /*calcoliamo la posizione*/
                pos = lseek(fd, 0L, SEEK_CUR);
                /*mandiamo la posizione al padre*/
                nw = write(pipedfp[i][1], &pos, sizeof(pos));
                if(nw != sizeof(pos))
                {
                    printf("La posizione di indice %d non è stata inviata correttamente ed è fallita\n",i);
                    exit(-1);
                }

                /*il figlio Pi riceve il carattere da sostituire inviato dal padre inviato dal padre*/
                nr = read(pipedpf[i][0], &cx, 1);
                /*adesso il figlio deve controllare che il padre non gli abbia mandato il carattere '\n'*/
                if(cx != '\n')
                {
                    /*per poter sovrascrivere il carattere , come richiesto dal padre, bisogna tornare indietro di una posizione*/
                    lseek(fd, -1L, SEEK_CUR);
                    write(fd, &cx,1);
                    ritorno++; /*Il figlio incrementa il valore di ritorno*/
                }
            }
        }
        exit(ritorno); /*ritorniamo il numero di posizione trovate*/
       } /*fino processo figlio*/
     } /*fine ciclo for*/
    
    /*codice padre*/
    /*dopo aver concluso il ciclo for di creazione si tutti i figli, si considera il resto del codice che ddeve eseguire il padre!*/
    /*padre*/
    /*chiude il lato della pipe che non usa*/

     for (i=0; i < N ; ++i)
    {
        close(pipedfp[i][1]);
        close(pipedpf[i][0]);
    }

    /*il padre deve ricevere le posizioni inviate da figli*/
    for(i = 0; i < N ; ++i)
    {
        nr = read(pipedfp[i][0], &pos, sizeof(pos));
        if(nr != sizeof(pos))
        {
            printf("La posizione di indice %d non è stata ricevura correttamente ed è fallita\n", i);
            exit(-1);
        }

        if(nr != 0)
        {
            /*per ogni posizione ricevuta, il padre deve riportare sullo stdout l'indice del figlio che 
        gli inviato la posizione, il nome del file in cui è stata trovata l'occorenza del carattere Cx 
        e la posizione ricevuta*/

        printf("Abbiamo trovato una occorenza del carattere %c nel file %s nella posizione %ld inviata dal figlio di indice %d\n", Cx, argv[i+1], pos, i);

        /*il padre deve chiedere all'utente il carattere con cui deve essere sostituita la specifica occorenza */
        read(0, &cx, 1);
        if(cx != '\n') read(0, &scarto, 1);

        write(pipedpf[i][1], &cx, 1); /*inviamo al figlio*/

        }

        

   
    }

    /*attesa della terminazione dei figli*/
    for (i=0; i < N; i++)
  	{
   		if ((pidFiglio = wait(&status)) < 0)
   		{
      			printf("Errore wait\n");
      			exit(6);
   		}
   		if ((status & 0xFF) != 0)
                	printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
   		else
   		{
                	ritorno=(int)((status >> 8) & 0xFF);
                    printf("Figlio con pid %d ha ritornato %d\n", pid , ritorno);
   		}
  	} 

    exit(0);

}