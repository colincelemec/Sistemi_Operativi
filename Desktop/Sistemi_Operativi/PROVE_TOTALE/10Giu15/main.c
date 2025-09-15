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

/*definizione del tipo di data Struct*/
typedef struct  
{
    int id; /*campo c1 che è l'indice d'ordine di un processo*/
    int occ; /*numero di occorenze del carattere del carattere Cx*/
} strutt;

int main(int argc, char **argv)
{
    /*---------VARIABILI LOCALI-------------------*/
    int *pid; /*pid per fork*/
    int N; /*numero di file passati sulla riga di comando*/
    int H; /*per l'ultimo parametro */
    char Cx; /*per il carattere chiesto dall'utente dal padre*/
    
    char c; /*per il carattere da cercare*/
    int fd; /*variabile per la open*/
    int i,j; /*per i cicli*/
    int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
    strutt S; /*struttura usata dal figlio corrente*/
    strutt pip; /*struttura usata dal figlio e per la pipe*/
    pipe_t *piped; /*array dinamico di pipe per communicazioni figli-padre*/
    int pidFiglio, status, ritorno; /*variabili per wait*/
   
    /*-----------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if(argc < 4)
    {
        printf("Errore sul numero di parametri. Ci vuole almeno 3 parametri\n");
        exit(1);
    }

    /*---------CALCOLARE IL VALORE DI N-----------------*/
    N = argc - 2; /* si deve togliere il nome dell'eseguibile e l'ultimo parametro H*/

    /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/

    /*Controllo che sia un numero intero strettamente positivo*/
        /*prima ,convertiamolo in un numero*/
    H=atoi(argv[argc - 1]);
    if(H < 0){
        printf("Errore; il secondo parametro %s non è un numero strettamente maggiore di 0\n", argv[argc - 1]);
        exit(2);
    }

    /*ALLOCAZIONE PID*/
    if((pid = (int *)malloc(N *sizeof(int))) == NULL)
    {
        printf("Errore allocazione pid\n");
        exit(3);
    }

    

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped = (pipe_t *)malloc(N * sizeof(pipe_t));
    if(piped == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe\n");
        exit(5);
    }

    /*creazione pipe*/
    for(i=0; i < N; ++i)
    {
        if(pipe(piped[i]) < 0)
        {
            printf("Errore nella creazione della pipe\n");
            exit(6);
        }
    }

    /*Il processo deve padre deve prima di tutto chiedere all'utente un carattere Cx*/
    read(0,&Cx,1);
    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("carattere letto %c\n", Cx);
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N);

    /*ciclo di generazione dei figli*/
    for(i=0; i<N ;i++)
    {/*inizio ciclo for*/
        /*CREAZIONE DEI PROCESSI FIGLI*/
        if((pid[i] = fork()) < 0)
        {
            printf("Errore nella fork di indice %d-esima\n", i);
            exit(7);
        }

        if(pid[i] == 0)
        { /*inizio processo figlio*/
             /*NEL CASO DI UNA PIPELINE*/
               /*chiusura dei lati delle pipe non usate nelle pipeline*/
               /*per capire la chiusura, fare riferimento al comento riportato per l'array pipes*/
               for( j=0; j<N; j++)
               {
                if(j!=i)
                    close (piped[j][1]);
                if((i == 0) || (j != i-1))
                    close (piped[j][0]);
               }

            printf("DEBUG-Sono il processo figlio con pid %d di indice %d e sono associato al file %s \n", getpid(), i, argv[i+1]);

            /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[i+1]);
                    exit(-1);
                }
                
                    /*inizializziamo la struttura*/
                    S.id = i;
                    S.occ = 0;

                    /*leggiamo un carattere una alla volta*/
                    while(read(fd, &c, 1))
                    {
                        if(c == Cx){ /*se abbiamo trovato il carattere*/
                            S.occ++; /*incrementiamo il contatore*/
                        }    
                    }

                    /*il figlio PO passa in avanti per ogni linea letta, una struct che
                        deve contenere i campi c1 e c2 con c1=0 e c2=numero di occorenze trovare*/

                    /*lettura da pipe dal figlio precedente dell'array di strutture per tutti i figli a parte il primo*/
                    if(i != 0)
                    {
                        nr=read(piped[i-1][0],&S,sizeof(strutt));
                      
                        /*verifichiamo che la lettura sia andata a buon fine*/
                        if(nr != sizeof(pip))
                        {
                            printf("Errore nella lettura da pipe[%d]\n", i);
                            exit(-1);
                        }
                        else{
                            if(S.occ >= pip.occ)
                            {
                                /*se il numero di occorenze non è minore dobbiamo passare avanti la struttura
                                altrimenti rimane valida la struttura corrente*/
                                S.id = pip.id;
                                S.occ = pip.occ;
                            }
                        }
                    }


                    /*tutti i figli mandano in avanti (cioè al figlio successivo, a parte l'ultimo che manda al padre) un array di strutture di dimensione fissa*/
                    
                        nw=write(piped[i][1], &S, sizeof(strutt));
                        /*verifichiamo che la struttura sia andata a buon fine*/
                        if(nw != sizeof(pip))
                        {
                            printf("Errore nella scrittura da pipe[%d]\n", i);
                            exit(-1);
                        }else
                        {
                            /*dopo l'invio, ripristino i dati*/
                            S.occ = 0;
                            S.id = i;
                        }
                    
                    
                    
                
            exit(0);
        } /*fine processo figlio*/
    }/*fine ciclo for*/

    /*padre*/
    /*NEL CASO DEL PIPELINE*/
    /*chiusura lati delle pipe non usati: tutti meno quello l'ultimo in lettura*/
    for(i=0; i<N; i++)
    {
        close(piped[i][1]);
        if(i != N-1) close (piped[i][0]);
    }

    /*il padre deve riceve le H strutture inviate*/
    for(j = 0; j < H ; ++j)
    {
         nr=read(piped[N-1][0],&pip,sizeof(S));
        /*controlla che sia letto correttamente*/
        if(nr != sizeof(S))
        {
            printf("Errore in lettura da pipe[N-1] per il padre\n");
            exit(-1);
        }
        printf("Ricevuto per la linea %d che il figlio di indice %d e pid %d ha trovato %d occorenze del carattere %c\n", j, pip.id, pid[pip.id],pip.occ,Cx);
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
                    printf("Figlio con pid %d ha ritornato %d\n", pid[i] , ritorno);
   		}
  	} 
    exit(0);
 }

