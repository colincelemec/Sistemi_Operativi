#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

/*IN QUESTO ESERCIZIO L'UNICA COSA DA RETTIFICARE è CHE NON ABBIAMO BISOGNO DI UN ARRAY DI PIPE
. SOLO UN PIPE FIGLIO-NIPOTE E UN ALTRO NIPOTE-FIGLIO*/

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
    pipe_t *piped; /*array dinamico di pipe per communicazioni figli-padre*/
    pipe_t p; /*singola pipe per communicazione figlio-nipote*/
    int pidFiglio, status, ritorno; /*variabili per wait*/
    char *File_creato; /*per il file da creare*/
    int Fout; /*per il file descriptor del file creato*/
    char c; /*per leggere i caratteri dal figlio*/
    char ch; /*per leggere i caratteri dal nipote*/
    int count = 0; /*per contare i caratteri letti*/
     /*-----------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if (argc < 3)
    {
        printf("Errore sul numero di parametri. Ci vuole almeo 2 parametri\n");
        exit(1);
    }

     /*---------CALCOLARE IL VALORE DI N-----------------*/
     N = argc - 1;
     /*CONTROLLO CHE N  SIA PARI*/
     if ( N % 2 != 0)
     {
        printf("Errore: N  non è pari");
        exit(2);
     }

    /*NEL CASO DI UN ARRAY DI N / 2 PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped = (pipe_t *)malloc((N / 2)* sizeof(pipe_t));
    if(piped == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe\n");
        exit(3);
    }

    /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
    for(i=0; i < N/2; i++)
    {
        if(pipe(piped[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(4);
        }
    }
    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N/2);
   
    /*ciclo di generazione dei figli*/
     for(i=0; i<N/2 ;i++)
     {/*inizio ciclo for*/

        /*CREAZIONE DEI PROCESSI FIGLI*/
         if((pid = fork()) < 0)
            {
                printf("Errore nella fork di indice %d-esima\n", i);
                exit(5);
            }

            if (pid == 0)
            { /*inizio processo figlio*/
            /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
                 /*NEL CASO DI N PIPE*/
                 for( j=0; j < N/2; j++)
                {
                /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                close(piped[j][0]);
                if(i != j)
                    close(piped[j][1]);
               }

               

               /*prima di tutto , dobbiamo creare il file Fout il cui nome deve risultare dalla concatenazione 
               della stringa "merge" e della stringa corrispondente a i*/
               /*bisogna allocare una stringa lunga come il nome del file + "merge"+ il terminatore della stringa*/
               File_creato = (char *)malloc(strlen(argv[i+1]) + 7);
               if(File_creato == NULL)
               {
                printf("Errore nella malloc\n");
               }
                /*copiamo il nome del file associato alla stringa*/
                strcpy(File_creato,argv[i+1]);
                strcat(File_creato, "merge");

                Fout = creat(File_creato, 0644);
                if(Fout < 0)
                {
                    printf("Errore nella creazione del file %s\n", File_creato);
                }

                /*creaimo il processo nipote*/
                if(pipe(p) < 0)
                {
                    printf("Errore nella creazione della pipe fra figlio e nipote per l'indice %d\n", i);
                    exit(-1);
                }

                /*ogni figlio crea un nipote*/
                if((pid = fork()) < 0)
                {
                    printf("Erroren nella creazione del nipote per l'indice %d\n", i);
                    exit(-1);
                }

                if(pid == 0)
                { /*inizio processo nipote*/

                    /*CODICE NIPOTE*/
                    /*FARE UN PRINTF PER DIRE COSA DEVE FARE IL NIPOTE*/
                    printf("DEBUG-SONO IL PROCESSO NIPOTE con di indice %d con pid %d e sono associato al file %s\n", i, getpid(), argv[N/2 + i + 1]);

                    /*NB. chiusura della pipe rimasta aperta di communicazione fra figlio-padre che il nipote non usa*/
                    close(piped[i][1]);

                    /*ridirezione dello stdin: il file si trova usando l'indice i incrementato di 1*/
                    close(0);
                    if ((fd=open(argv[N/2+i+1], O_RDONLY)) < 0)
                    {
                        printf("Errore nella open del file %s\n", argv[N/2+i+1]);
                        exit(-1);
                    }
                    /*ogni nipote deve simulare il piping dei comandi nei confronti del figlio e quindi deve chiudere lo stdout e quindi usare la dup sul lato di scrittura della propria pipe*/
                    close(1);
                    dup(p[1]);
                    /*ogni nipote adesso può chiudere entrambi i lati della pipe: il lato 0 non viene usato e il lato 1 viene usato tramite lo stdout*/
                    close(p[0]);
                    close(p[1]);

                    /*ridirizione dello stderr su /dev/null*/
                    close(2);
                    open("/dev/null", O_WRONLY);


                    /*deve leggere dal suo file associato un carattere alla volta*/
                    while(read(fd, &c, 1)) /*leggiamo un carattere alla volta*/
                    {
                        count++; /*incrementiamo il numero di caratteri letti*/
                        /*dobbiamo verificare che c sia il primo carattere*/
                        if ( count == 1) /*se siamo sul primo carattere letto*/
                        {
                            /*lo dobbimao scrivere su Fout*/
                           write(Fout, &c, 1);
                            /*dobbiamo communicare al figlio il primo carattere letto */
                            nw = write(p[1], &c, 1);
                            /*verifichimao che la scrittura sia andata a buon fine*/
                            if (nw != sizeof(c))
                            {
                                printf("Errore nella scrittura sulla pipe\n"),
                                exit(-1);
                            }
                        }
                    }

                    /*devo ricevere il carattere inviato dal figlio*/
                    nr = read(p[0], &ch, 1);
                    /*verifichiamo che la lettura sia andata a buon fine*/
                    if( nr != sizeof(ch))
                    {
                        printf("Errore nella lettura\n");
                        exit(-1);
                    }

                    exit(ch);

                } /*fine processo nipote*/
                /*codice figlio*/
                /*ogni figlio deve chiudere il lato che non usa della pipe di communicazione con il nipote*/
                close(p[1]);

                /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PROCESSO FIGLIO*/

                /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[i+1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[i+1]);
                    exit(-1);
                }

               printf("DEBUG-Sono il processo figlio di indice %d con pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);

               /*deve leggere dal suo file associato un carattere alla volta*/
                    while(read(fd, &ch, 1)) /*leggiamo un carattere alla volta*/
                    {
                        count++; /*incrementiamo il numero di caratteri letti*/
                        /*dobbiamo verificare che c sia il primo carattere*/
                        if ( count == 1) /*se siamo sul primo carattere letto*/
                        {
                            /*lo dobbimao scrivere su Fout*/
                           write(Fout, &ch, 1);
                            /*dobbiamo communicare al nipote il primo carattere letto */
                            nw = write(p[1], &ch, 1);
                            /*verifichimao che la scrittura sia andata a buon fine*/
                            if (nw != sizeof(ch))
                            {
                                printf("Errore nella scrittura sulla pipe\n"),
                                exit(-1);
                            }
                        }
                        

                    }
                    /*devo ricevere il caratterre inviato dal nipote*/
                    nr = read(p[0], &c, 1);
                    /*verifichiamo che la lettura sia andata a buon fine*/
                    if(nr != sizeof(c))
                    {
                        printf("Errore nella lettura\n");
                        exit(-1);
                    }

                ritorno=-1;
                if((pid = wait(&status)) < 0)
                    printf("errore nella wait\n");
                else 
                    if((status & 0xFF) != 0)
                        printf("Nipote con pid %d terminato in modo anomalo\n", pid);
                    else 
                        printf("DEBUG-Il nipote con pid=%d ha ritornato %d\n", pid , ritorno=(int)((status >> 8) & 0xFF));
                        exit(ritorno);
            } /*fine processo figlio*/
     } /*fine ciclo for*/

    /*dopo aver concluso il ciclo for di creazione si tutti i figli, si considera il resto del codice che ddeve eseguire il padre!*/
    /*padre*/
    /*chiude il lato della pipe che non usa*/

    /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (i=0; i < N/2 ; ++i)
    {
        close(piped[i][1]);
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