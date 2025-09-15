/*Questa soluzione non ha bisogno di usare array dinamici dato che il numero di processi è noto staticamente e pari al 
numero di lettere minuscole dall'alfaneto inglese (26 lettere)*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#define N 26 /*numero di figli da creare*/

/*definizione del TIPO pipe_t come array di 2 interi*/
typedef int pipe_t[2];

/*definizione del tipo di data Struct*/
typedef struct 
{
    char ch; /*campo v1 del testo*/
    long int occ; /*campo v2 del testo*/
} data;

/*funzione bubblesort*/

void bubbleSort(data *v, int dim)
{
   int i;
   data tmp;
   int ordinato = 0;
	while(dim > 0 && !ordinato)
	{
		ordinato = 1;
		for(i=0; i < dim-1; i++)
		{
			if(v[i].occ > v[i+1].occ)
			{
				tmp = v[i];
				v[i] = v[i+1];
				v[i+1] = tmp;
				ordinato = 0;
			}
		}
		dim--;
	}
}

int main(int argc, char **argv)
{
    /*---------VARIABILI LOCALI-------------------*/
    int pid[N]; /*array STATICO di process identifier dei processi generati tramite fork: l'array di pid serve perché il padre deve ricavare il pid dall'indice del processo che avrà generato la specifica struttura*/
    char c; /*per leggere dal file*/
    pipe_t pipes[N]; /*array STATICO di pipe usate a pipeline*/
    int i,j; /*per i cicli*/
     int fd; /*file descriptor del file aperto da ogni figlio in lettura*/
     data d[N]; /*array STATICO di strutture che i figli si passano nella pipeline, fino al padre*/
     char C; /*carattere da cercare da parte di ogni figlio*/
     long int cont_occ; /*per conteggio corrente*/
     int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
     int pidFiglio, status, ritorno; /*per la wait*/
    /*------------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if(argc != 2)
    {
        printf("Errore sul numero di parametri.Ci vuole esattamente 1 parametro\n");
        exit(1);
    }

    /*Qui non abbiamo bisogno di calcolare il numero di N perché vale 26*/

    /*creazione delle 26 pipe, usate in pipeline*/
    for(i=0; i<N; ++i)
    {
        if(pipe(pipes[i]) < 0)
        {
            printf("Errore nella creazione delle pipe\n");
            exit(2);
        }
    }

     /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N);

    /*dati che il padre deve stampare il pid dei figli una volta ricevuto l'array e ordinatolo, si devono memorizzare i pid dei figli*/
    for(i=0; i<N; i++)
    {
        if((pid[i] = fork()) < 0)
        {
            printf("Errore nella creazione figlio %d-esimo\n",i);
            exit(3);
        }

        if(pid[i] == 0)
        {
            /*codice del figlio*/
            /*definiamo a quale carattere è associato questo figlio*/
            C = 'a'+i;
             /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL PROCESSO FIGLIO*/
             printf("DEBUG-Sono il figlio di indice %d e pid %d e sto per creare il carattere %c nel file %s\n", i, getpid(), C, argv[1]);

             /*chiusura dei lati delle pipe non usate nelle pipeline*/
               /*per capire la chiusura, fare riferimento al comento riportato per l'array pipes*/
                 for( j=0; j < N; j++)
                {
            
                if(i != j)
                    close(pipes[j][1]);
                if((i == 0) || (j != i-1))
                    close(pipes[j][0]);
               }

               /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[1]);
                    exit(-1);
                }

                cont_occ = 0L; /*inizializziamo il conteggio*/
                /*ogni figlio deve leggere il proprio file , un carattere alla volta*/
                while(read(fd,&c,1))
                {
                    /*cerchiamo il carattere*/
                    if(c==C)
                        cont_occ++; /*trovato il carattere e quindi si incrementa il conteggio*/
                }

                /*lettura da pipe dal figlio precedente dell'array di strutture per tutti i figli a parte il primo*/
                if(i!=0)
                {
                    nr=read(pipes[i-1][0],d,sizeof(d));
                    /*d è un arrar statico e quindi si può usare il sizeof per ricavare la lunghezza dell'array che sarà uguale a N * sizeof(data) */
                    /*N.B: in caso di pipeline è particolarmente importante che la lettura sia andata a buon fine e che quindi la pipeline non si sia 'rotta', si deve quindi controllare il valore di ritorno della read da pipe!*/
                    if(nr != sizeof(d))
                    {
                        printf("Errore in lettura da pipe[%d]\n", i);
                        exit(-1);
                    }
                }
                /*inseriamo le informazioni del figlio corrente nella posizione giusta e quindi nella posizione i-esima*/
                d[i].ch = C;
                d[i].occ = cont_occ;

                /*tutti i figli mandano in avanti (cioè al figlio successivo, a parte l'ultimo che manda al padre) un array di strutture di dimensione fissa*/
                nw=write(pipes[i][1], d, sizeof(d));
                /*verifichiamo che la scrittura sia andata a buon fine*/
                if(nw != sizeof(d))
                {
                    printf("Errore in scrittura da pipe[%d]\n", i);
                    exit(-1);
                }
                /*torniamo l'ultimo carattere letto (che sarà chiaramente uguale per tutti i figli)*/
                exit(c);
        }
    }/*fine for*/

    /*codice del padre*/
    /*chiusura lati delle pipe non usati: tutti meno quello l'ultimo in lettura*/
    for(i=0; i<N; i++)
    {
        close(pipes[i][1]);
        if(i != N-1) close (pipes[i][0]);
    }

    /*il padre deve leggere l'array di struttura che gli arriva dall'ultimo figlio*/
    /*quindi al padre arriva una singola informazione, rappresentata però da un array che quindi contiene più elementi*/
    nr=read(pipes[N-1][0],d,sizeof(d));
    /*controlla che sia letto correttamente*/
    if(nr != sizeof(d))
    {
        printf("Errore in lettura da pipe[N-1] per il padre\n");
        exit(4);
    }
    else 
    {
        /*ordiniamo l'array ricevuto, secondo il campo occ delle varie strutture*/
        bubbleSort(d,N);
        /*il padre deve stampare quanto indicato rispettando la specifica*/
        for(i=0; i<N; i++)
        {
            printf("Il figlio di indice %d e pid %d ha trovato %ld occorenze del carattere %c\n", d[i].ch-'a', pid[(d[i].ch-'a')], d[i].occ, d[i].ch);
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
                    printf("Figlio con pid %d ha ritornato il carattere %c\n", pidFiglio , ritorno);
   		}
  	} 

    exit(0);
}
