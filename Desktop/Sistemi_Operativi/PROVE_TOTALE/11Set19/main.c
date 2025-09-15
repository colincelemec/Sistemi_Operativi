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

/*definizione del TIPO pipe_t come array di 2 interi*/
typedef int pipe_t[2];


int main(int argc, char **argv)
{   /*---------VARIABILI LOCALI-------------------*/
    int pid; /*pid per fork*/
    int M; /*numero di file passati sulla riga di comando*/
    int fd; /*variabile per la open*/
    int i,j; /*per i cicli*/
    int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
    char c; /*per leggere dal file*/
    char Cp; /*carattere da controllare per ogni file in poizione pari*/
    char Cd='\0'; /*carattere da controllare per ogni file in posizone dispari*/
    int count=0; /*per il numero di caratteri letti dal file*/
    pipe_t *piped_dp; /*array di pipe per la communicazione dal processo Pd al processo Pp */
    char *Fcreato; /*per il file che dobbiamo creare*/
    int fdw; /*per il file descriptor del file creato*/
    int pidFiglio, status, ritorno; /*variabili per wait*/
   
    /*-----------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if(argc < 3)
    {
        printf("Errore sul numero di parametri. Ci vuole almeno 2 parametri che devono essere nomi di file");
        exit(1);
    }

    /*---------CALCOLARE IL VALORE DI M-----------------*/
    M = argc - 1;
    /*CONTROLLIAMO CHE M SIA PARI*/
    if( M % 2 != 0)
    {
        printf("M deve essere pari e in questo caso non lo è perché è dispari\n");
        exit(2);
    }

    /*NEL CASO DI UN ARRAY DI M PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped_dp = (pipe_t *)malloc(M/2* sizeof(pipe_t));
    if(piped_dp == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe\n");
        exit(3);
    }

     /*CREAZIONE DI QUESTI M PIPE ALLOCATI*/
    for(i=0; i< M/2; i++)
    {
        if(pipe(piped_dp[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(4);
        }
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), M);

     /*ciclo di generazione dei figli*/
     for(i=0; i<M ;i++)
     {/*inizio ciclo for*/
        /*CREAZIONE DEI PROCESSI FIGLI*/
        if((pid = fork()) < 0)
            {
                printf("Errore nella fork di indice %d-esima\n", i);
                exit(5);
            }

        if(pid == 0)
        { /*inizio processo figlio*/

            if(i % 2 == 1) 
            { /*se è in posizone dispari*/
                 /*NEL CASO DI N PIPE*/
                 for( j=0; j < M/2; j++)
                {
                /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                close(piped_dp[j][0]);
                if(i/2 != j)
                    close(piped_dp[j][1]);
               }
                printf("DEBUG-Sono il primo processo figlio con di indice dispari %d con pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);

                /*apriamo il file */
                if((fd = open(argv[i+1], O_RDONLY)) < 0)
                {
                    printf("Errore nell'apertura del file %s\n", argv[i+1]);
                    exit(-1);
                }

                
                /*ogni processo figlio deve leggere i caratteri del file file associato Ff*/
                while(read(fd,&c,1)) /*leggiamo un carattere alla volta*/
                {
                    ++count; /*incrementiamo il numero di caratteri letti*/
                    if(c == Cd) /*se leggiamo il carattere Cd*/
                    {
                        /*communichiamolo al processo Pp*/
                        nw = write(piped_dp[i/2][1], &Cd, sizeof(Cd));
                        if(nw != sizeof(Cd))
                        {
                            printf("Errore nella scrittura su pipe\n");
                            exit(-1);
                        }
                    }
                }

            }/*fine processo posizione dispari*/
            else
            { /*siamo qui se il file è in posizione pari*/
                /*NEL CASO DI N PIPE*/
                 for( j=0; j < M; j++)
                {
                /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                close(piped_dp[j][0]);
                if(i != j)
                    close(piped_dp[j][1]);
               }

               printf("DEBUG-Sono il primo processo figlio con di indice pari %d con pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);
               /*apriamo il file */
                if((fd = open(argv[i+1], O_RDONLY)) < 0)
                {
                    printf("Errore nell'apertura del file %s\n", argv[i+1]);
                    exit(-1);
                }
                /*creaimo un file Fcreato i cui nome sia la concatenazione del nome del file associato Ff con la stringa ".MINORE"*/
                
                /*bisogna allocare una stringa lunga come il nome del file + il carattere '.' + i caratteri della parola MINORE(6) + IL TERMINTAORE DELLA STRINGA*/
                Fcreato = (char *)malloc(strlen(argv[i+1]) + 10);
                if(Fcreato == NULL)
                {
                    printf("Errore nella malloc\n");
                    exit(0);
                }
                /*copiamo il nome del file associato al figlio*/
                strcpy(Fcreato,argv[i+1]);
                strcat(Fcreato,".MINORE");
                fdw = creat(Fcreato,0644);
                if(fdw < 0)
                {
                    printf("Impossibile creare il file %s\n", Fcreato);
                }

                /*ogni processo figlio deve leggere i caratteri del file file associato Ff*/
                while(read(fd,&c,1)) /*leggiamo un carattere alla volta*/
                {
                    ++count; /*incrementiamo il numero di caratteri letti*/
                    if(c == Cp) /*se leggiamo il carattere Cp*/
                    {
                        /*il processo Pp legge il suo proprio carattere*/
                        nr = read(piped_dp[i/2][0], &Cp, sizeof(Cp));
                        if(nr != sizeof(Cp))
                        {
                            printf("Errore nella lettura del carattere da pipe\n");
                            exit(-1);
                        }
                    }
                }

                /*riceviamo il carattere Cd inviato dal primo processo*/
                nr = read(piped_dp[i][0], &Cd, sizeof(Cd));
                /*verifichiamo se il carattere è stato ben ricevuto*/
                if( nr != sizeof(Cd))
                {
                    printf("Errore nella lettura\n");
                    exit(-1);
                }

                /*il processo Pp deve confrontare il suo carattere Cp con Cd*/
                if( Cp < Cd)
                {
                    /*lo scriviamo sul file Fcreato*/
                    write(fdw, &Cp, 1);
                }else{
                    /*alrimenti*/
                    write(fdw, &Cd,1);
                }

            } /*fine processo con file in posizione pari*/

            exit(count);
        }/*fine processo figlio*/


    }/*fine ciclo for*/

  /*padre*/
    /*chiude il lato della pipe che non usa*/


    /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (i=0; i < M ; ++i)
    {
        close(piped_dp[i][1]);
        close(piped_dp[i][0]);
    }

    /*attesa della terminazione dei figli*/
    for (i=0; i < M; i++)
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