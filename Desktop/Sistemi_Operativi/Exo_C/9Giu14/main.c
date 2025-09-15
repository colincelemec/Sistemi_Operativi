#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/*definizione del TIPO pipe_t come array di 2 interi*/
typedef int pipe_t[2];

int main(int argc, char **argv)
{
    /*---------Variabili locali------------*/
    int pid; /*process identifier per le fork()*/
    int N; /*numeri di file passati sulla riga di comando (uguale al numero di figli)*/
    pipe_t *piped; /*array dinamico di pipe per communicazioni figli-padre*/
    pipe_t p; /*una sola pipe per ogni coppia figlio-nipote*/
    int i,j; /*indici per i cicli*/
    char numero[11]; /*array di caratteri per memorizzare la stringa corrispondente al numero di righe*/
    int valore; /*variabile che viene communicata da ogni figlio al padre e che contiene la conversione della stringa in numero*/
    int ritorno; /*variabile che viene ritornata da ogni figlio al padre e che contiene il ritorno del nipote*/
    long int somma=0; /*variabile usata dal padre per calcolare la somma di tutte le lunghezze communicate dai figli*/
    int status; /*variabile di stato per la wait*/
    /*----------------------------------------*/

     /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if(argc < 3)
    {
        printf("Errore: Necessitano almeno 2 parametri\n");
        exit(1);
    }

    /*---------CALCOLARE IL VALORE DI N-----------------*/
    N = argc - 1;

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped = (pipe_t *)malloc(N * sizeof(pipe_t));
    if(piped == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe\n");
        exit(2);
    }

    /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
    for(i=0; i < N; i++)
    {
        if(pipe(piped[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice n = %d\n", i);
            exit(3);
        }
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N);

    for(i=0; i<N ;i++)
    {
    /*CREAZIONE DEI PROCESSI FIGLI*/
        if((pid = fork()) < 0)
        {
            printf("Errore nella fork\n");
            exit(5);
        }

        if(pid == 0) /*FIGLIO*/
        {
            /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL PROCESSO FIGLIO*/
            printf("DEBUG-Sono il processo figlio di indice %d e pid %d sto per creare il nipote che calcolerà il numero di linee del file %s\n", i, getpid(), argv[i+1]);
            /*in caso di errore nel figlio o nel nipote, decidiamo di tornare -1 che verrà interpretato dal padre come 255 e quindi un valore non ammissibile!*/

             /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
             /*NEL CASO DI N PIPE*/
                 for( j=0; j < N; j++)
                {
                /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                close(piped[j][0]);
                if(j != i)
                    close(piped[j][1]);
               }

                /*per prima cosa creiamo la pipe di communicazione fra figlio e nipote*/
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
                {
                    /*codice del nipote*/
                    /*FARE UN PRINTF PER DIRE COSA DEVE FARE IL NIPOTE*/
                    printf("DEBUG-Sono il processo nipote del figlio di indice %d e pid %d sto per calcolare il numero di linee del file %s\n", i, getpid(), argv[i+1]);
                    /*in caso di errore nel figlio o nel nipote, decidiamo di tornare -1 che verrà interpretato dal padre come 255 e quindi un valore non ammissibile!*/

                    /*NB. chiusura della pipe rimasta aperta di communicazione fra figlio-padre che il nipote non usa*/
                    close(piped[i][1]);

                    /*ridirezione dello stdin: il file si trova usando l'indice i incrementato di 1*/
                    close(0);
                    if (open(argv[i+1], O_RDONLY))
                    {
                        printf("Errore nella open del file %s\n", argv[i+1]);
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

                    /*il nipote diventa il comando wc -l*/
                    execlp("wc", "wc", "-l", (char *)0);

                    /*NON si dovrebbe mai tornare qui !*/
                    exit(-1);
                }
                /*codice figlio*/
                /*ogni figlio deve chiudere il lato che non usa della pipe di communicazione con il nipote*/
                close(p[1]);

                /*adesso il figlio legge dalla pipe un carattere alla volta, fino a che ci sono caratteri inviati dal nipote tramite il comando-filtro wc*/
                j=0; /*inizializziamo l'indice del buffer numero*/

                while(read(p[0], &(numero[j]), 1))
                {
                    j++; /*incrementiamo l'indice*/
                }
            
                /*all'uscita di questo ciclo white, nell'array numero ci saranno tutti i caratteri numerici corrispondenti al numero di linee del file e come ultimo carattere il terminatore di linea*/
                /*trasformiamo l'array di char in stringa sostituendo '\n' il terminatore di stringa, controllando però di avere letto in effetti qualcosa*/
                if(j != 0) /*se il figlio ha letto qualcosa*/
                {
                    numero[j-1]='\0';
                    /*convertiamo la stringa nel numero che bisogna comunicare al padre*/
                    valore=atoi(numero);
                }
                else
                {
                    /*questo è il caso che il nipote sia incorso in un errore e che quindi non abbia eseguito il wc -l*/
                    valore=0; /*se il figlio non ha letto nulla, inviamo 0*/
                }

                /*il figlio comunica al padre*/
                write(piped[i][1], &valore, sizeof(valore));
                /*il figlio deve aspettare il nipote per restituire il valore al padre*/
                /*se il nipote è terminato in modo anomalo decidiamo di tornare -1 e verrà interpretato come 255 e quindi segnalando questo problema al padre*/
                ritorno=-1;
                if((pid = wait(&status)) < 0)
                    printf("errore nella wait\n");
                else 
                    if((status & 0xFF) != 0)
                        printf("Nipote con pid %d terminato in modo anomalo\n", pid);
                    else 
                        printf("DEBUG-Il nipote con pid=%d ha ritornato %d\n", pid , ritorno=(int)((status >> 8) & 0xFF));
                        exit(ritorno);

        }
    }

    /*codice del padre*/
    /*il padre chiude i lati delle pipe che non usa*/
    /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (i=0; i < N ; ++i)
    {
        close(piped[i][1]);
    }

    /*il padre recupere le informazioni dai figli in ordine di indice*/

    for(i=0; i < N; ++i)
    {
        /*il padre recupera tutti i valori interi dai figli*/
        read(piped[i][0], &valore, sizeof(valore));
        printf("DEBUG-Il figlio di indice %d ha comunicato il valore %d per il file %s\n", i, valore, argv[i+1]);
        somma=somma+(long int)valore;
    }
    printf("La somma risultante derivante dai valori comunicati dai figli è %ld\n", somma);

    /*Il padre aspetta i figli*/
     /*attesa della terminazione dei figli*/
    for (i=0; i < N; i++)
  	{
   		if ((pid = wait(&status)) < 0)
   		{
      			printf("Errore wait\n");
      			exit(5);
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