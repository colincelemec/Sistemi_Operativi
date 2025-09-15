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

int mia_random(int n)
{
    int casuale;
    casuale = rand() % n; /*con rand() viene calcolato un numero pseudo casuale e con l'operazione 
                            modulo n si ricava un numero casuale compreson fra 0 e n-1*/
    return casuale;
}

int main(int argc , char **argv)
{
    /*---------VARIABILI LOCALI-------------------*/
    int pid; /*pid per fork*/
    int N; /*numero di file passati sulla riga di comando*/
    int H; /*numero passato come ultimo parametro e che rappresenta la lunghezza in linee dei file passati sulla riga di comando */
    int fdout; /*file descriptor per creazione file da parte del padre*/
    int fd; /*file descriptor per apertura file*/
    pipe_t *pipedFP; /*array dinamico di pipe per communicazioni figli-padre*/
    pipe_t *pipedPF; /*array dinamico di pipe per communicazioni padre-figli*/
     int i,j; /*per i cicli*/
     char linea[255]; /*array di caratteri per memorizzare la linea, supponendo una lunghezza massima di ogni linea di 255 caratteri compreso il terminatore di linea*/
     int valore; /*variabile che viene usata dal padre per recuperare il valore communicato da ogni figlio e che contiene la lunghezza della linea corrente*/
     int giusto; /*variabile che viene usata dal padre per salvare per ogni linea il valore inviato dal figlio selezionato in modo random*/
     int r; /*variabile usata dal padre per calcolare i valori random e dal figlio per ricevere il numero del padre*/
     int ritorno=0; /*variabile che viene ritornata da ogni figlio al padre e che contiene il numero di caratteri scritti sul file*/
     int status; /*variabile di stato per a wait*/
     /*-----------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if(argc < 6)
    {
        printf("Errore sul numero di parametri: Ci vuole almeno 5 parametri: 4 nomi di file e un numero intero strettamente positivo\n");
        exit(1);
    }

    /*---------CALCOLARE IL VALORE DI N-----------------*/
    N = argc - 2;

    /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/
    /*Controllo che sia un numero intero strettamente positivo*/
    /*prima ,convertiamolo in un numero*/
    H = atoi(argv[argc-1]);

    if((H <= 0) || (H >= 255))
    {
        printf("Errore ne numero passato %s\n", argv[argc-1]);
        exit(2);
    }

    /*inizializziamo il seme per la generazione random dei numeri*/
    srand(time(NULL));

    /*creiamo il file in /tmp */
    if((fdout = open("/tmp/creato",O_CREAT|O_WRONLY|O_TRUNC, 0644)) < 0)
    {
        printf("Errore nella creazione del file %s\n", "/tmp/creato");
        exit(3);
    }

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    /*QUINDI IN QUESTO CASO ALLOCHIAMO due array di N pipe*/
    pipedFP=(pipe_t *) malloc(N * sizeof(pipe_t));
    pipedPF=(pipe_t *) malloc(N * sizeof(pipe_t));

    if((pipedFP == NULL) || pipedPF == NULL)
    {
        printf("Errore nell'allocazione della memoria per le due serie di N pipe\n");
        exit(4);
    }

    /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
    /*CREEREMO DUE QUINDI UNO PER IL PADRE-FIGLIO E L'LATRO PER IL FIGLIO-PADRE*/
    for(i=0; i<N; ++i){
        if(pipe(pipedFP[i]) < 0)
        {
            printf("Errore nella creazione della pipe %d-esima figli-padre\n",i);
            exit(5);
        }

        if(pipe(pipedPF[i]) < 0)
        {
            printf("Errore nella creazione della pipe %d-esima padre-figli\n",i);
            exit(6);
        }
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N);

    /*ciclo di generazione dei figli*/
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
            printf("DEBUG-Sono il figlio di indice %d con pid %d e sono associato al file %s\n", i, getpid(), argv[i+1]);
             /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
                 /*NEL CASO DI N PIPE*/
                 for( j=0; j < N; j++)
                {
                /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                close(pipedFP[j][0]);
                close(pipedPF[j][1]);
                if(i != j)
                    close(pipedFP[j][1]);
                    close(pipedPF[j][0]);
               }

              /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PROCESSO FIGLIO*/

              /*apriamo il file associato in sola lettura*/
              if((fd = open(argv[i+1],O_RDONLY)) < 0)
              {
                printf("Errore nella open del file %s\n", argv[i+1]);
                exit(-1);
              }
                /*adesso il figlio legge dal file una linea alla volta*/
                /*inizializziamo l'indice dei caratteri letti per ogni singola linea*/
                j=0; /*riusiamo la variabile j*/
                while(read(fd, &(linea[j]), 1)){
                    if(linea[j]=='\n'){
                    /*dobbiamo mandare al padre la lunghezza della linea corrente compreso il terminatore di linea (come int) e quindi incrementiamo j*/
                    j++;
                    write(pipedFP[i][1], &j, sizeof(1));
                    read(pipedPF[i][0], &r, sizeof(r));

                        if(r < j)
                        {
                            /*dato che si riceve un indice che varia da 0 alla lunghezza - 1 scelta dal padre
                            e dato che j rappresenta la lunghezza della linea corrente (compreso il terminatore
                            di linea), l'indice ricevuto per essere ammissibile deve essere strettamente minore di j;*/
                            /*stampa di controllo*/
                            printf("indice %d sto per scrivere carattere %c nel file\n", i, linea[r]);
                            /*ogni figlio usa, per scrivere sul file, il valore di fdout ottenuto per copia dal padre e condivide l 'I/O pointer con il padre e tutti i fratelli*/
                            write(fdout,&(linea[r]), 1);
                            /*il figlio incrementa il valore di ritorno*/
                            ritorno++;
                        }else{
                            j=0; /*azzeriamo l'indice per la prossima linea*/
                        }
                    }else{
                        j++; /*incrementiamo sempre l'indice della linea*/
                    }
                }

                exit(ritorno);
            }
        }

    /*codice del padre*/
    /*chiude il lato della pipe che non usa*/
    /*NEL CASO DI UNA ARRAY DI N PIPE*/
    for (i=0; i < N ; ++i)
    {
        close(pipedFP[i][1]);
        close(pipedPF[i][0]);
    }

    /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PADRE*/
    /*il padre deve recuperare le informazioni dai figli: prima in ordine di linee 
    e quindi in ordine di indice*/
    for(j=0; j<=H; j++)
    {
        /*il padre calcola in modo random l'indice del figlio di cui considererà il valore
        inviato: questa randomizzazione è opportuno che venga fatta prima di recuperare i valori in modo da indentificare il
        valore che dovrà essere usato per la seconda randomizzazione! */
        r=mia_random(N);
        /*stampa di controllo*/
        printf("DEBUG-Il numero generato in modo random (per la linea %d) compreso fra 0 e %d per selezionare il figlio è %d\n", j, N-1,r);
        
        for(i=0; i<N; ++i)
        {
            /*il padre recupera TUTTI i valori interi dai figli*/
            read(pipedFP[i][0], &valore, sizeof(valore));
            /*ma si salva solo il valore del figlio identificato in modo random*/
            if(i == r){
                giusto = valore;
            }
        }

        /*il padre calcola in modo random l'indice della linea che invierà a tutti i figli*/
        r = mia_random(giusto);
        /*stampa di controllo*/
        printf("DEBUG-Il numero generato in modo random compreso fra 0 e %d per selezionare l'indice della linea %d-esima è %d\n", giusto-1,j,r);

        /*il padre deve inviare a tutti i figli questo indice*/
        for(i=0; i < N; i++)
        {
            write(pipedPF[i][1], &r, sizeof(r));
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






