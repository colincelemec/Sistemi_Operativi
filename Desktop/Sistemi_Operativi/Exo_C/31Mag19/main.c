#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*definizione del TIPO pipe_t come array di 2 interi*/
typedef int pipe_t[2];

/*definizione del TIPO Struct per le singole strutture dati, che verrano inviate sulle pipe da parte dei figli al padre*/
typedef struct 
{
    int pid_nipote; /*campo c1 del testo*/
    int lung_linea; /*campo c2 del testo*/
    char linea_letta[250]; /*campo c3 del testo*/
} Strut;

int main(int argc, char **argv)
{
        /*---------VARIABILI LOCALI-------------------*/
        int pid; /*pid per fork*/
        int N; /*per il numero di file passati sulla riga di comando*/
        pipe_t *piped; /*array dinamico di pipe per communicazioni figli-padre*/
        pipe_t p; /*singola pipe per communicazione figlio-nipote*/
        int i,j; /*indici per i cicli*/
        int nr; /*variabile per il valore di ritorno della read*/
        Strut S; /*struttura che viene ritornata da ogni figlio al padre*/
        int ritorno, status; /*per la wait*/
        /*-----------------------------------------------*/

        /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
        if (argc < 4)
        {
            printf("Errore nel numero di parametro. Ci vuole almeno 3 parametri!\n");
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
    for(i=0; i< N; i++)
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
                exit(4);
            }

            if(pid == 0)
            {   /*CODICE FIGLIO*/

                /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
                /*NEL CASO DI N PIPE*/
                 for( j=0; j < N; j++)
                 {
                    /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                    close(piped[j][0]);
                    if(i != j)
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
                {   /*CODICE NIPOTE*/
                    /*FARE UN PRINTF PER DIRE COSA DEVE FARE IL NIPOTE*/
                    printf("DEBUG-Sono il processo nipote del figlio di indice %d e pid %d e sto per eseguire il comando sort -f per il file %s\n", i, getpid(), argv[i+1]);
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

                    /*il nipote diventa il comando sort*/
                    execlp("sort", "sort", "-f", argv[i+1]);
                    /*NON si dovrebbe mai tornare qui !*/
                    /*Usiamo perror che scrive su stdout error, dato che lo standard output è collegato alla pipe*/
                    perror("Problemi di esecuzione del comando sort da parte del nipote\n");
                    exit(-1);

                }

                /*codice figlio*/
                /*ogni figlio deve chiudere il lato che non usa della pipe di communicazione con il nipote*/
                close(p[1]);

                 /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PROCESSO FIGLIO*/
                /*ogni processo figlio deve ricevere solo la prima linea inviata dal nipote e deve inviare al padre la struttura di dati*/
                S.pid_nipote=pid; /*inizializziamo il campo con il pid del processore nipote*/
                /*adesso il figlio legge dalla prima linea*/
                j=0;
                while(read(p[0], &(S.linea_letta[j]), 1)) /*leggiamo dalla linea ogni carattere alla volta*/
                {
                    if(S.linea_letta[j] == '\n') /*se siamo arrivati alla fine di una linea*/
                    { 
                        /*aggiorniamo il campo con la lunghezza in caratteri della linea*/
                        S.lung_linea = j+1;
                        /*nell'array linea_letta abbiamo la PRIMA linea ricevuta*/
                        break; /*appena leggiamo una linea che sarà la prima, usciamo dal ciclo di lettura*/
                        /*NB: non serve azzerare j dato che usciamo dopo la lettura della prima linea*/
                    }else 
                        j++; /*incrementiamo l'indice della linea*/
                }
                /*il figlio communica al padre la struttura di dati*/
                write(piped[i][1],&S, sizeof(S));

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

        /*CODICE DEL PADRE*/
        /*chiude il lato della pipe che non usa*/
        /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (i=0; i < N ; ++i)
    {
        close(piped[i][1]);
    }

    /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PADRE*/

    /*il padre recupera le informazioni dai figli: in ordine di indice*/

    for(i=0; i < N; i++)
    {
        nr=read(piped[i][0], &S, sizeof(S));
        if(nr !=  0)
        {
            /*il padre deve trasformare in una stringa la linea ricevuta nel campo c3 da ogni struttura*/
            S.linea_letta[S.lung_linea]='\0';
            /*a questo punto la stampa della linea può essere effettuata con il formato %s*/
            printf("il nipote con pid %d ha letto dal file %s questa linea '%s' che ha lunghezza (compreso il termimatore) di %d caratteri:\n", S.pid_nipote, argv[i+1], S.linea_letta, S.lung_linea);
        }
    }

    /*attesa della terminazione dei figli*/
    for (i=0; i < N; i++)
  	{
   		if ((pid = wait(&status)) < 0)
   		{
      			printf("Errore wait\n");
      			exit(6);
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
