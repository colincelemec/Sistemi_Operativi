/* Soluzione della parte C del compito del 15 Febbraio 2017 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

typedef int pipe_t[2];

int main (int argc, char **argv)
{
int N; 		/* numero di file */
int X; 		/* numero di linee */
int pid;	/* pid per fork */
pipe_t *pipes;	/* array di pipe usate per la comunicazione dai figli al padre */
int i,j; 	/* contatori */
int fd; 	/* file descriptor */
int pidFiglio, status, ritorno;	/* per valore di ritorno figli */
char *primiCarNum; 	/* array dinamico di caratteri che i figli passeranno al padre */
char ch, max;  	/* carattere letto da ogni figlio dalle linee del file e carattere massimo calcolato dal padre */
int nr,nw;		/* variabili per salvare valori di ritorno di read/write da/su pipe */
int primo;	/* variabile per identificare il primo carattere numerico trovato per ogni linea */
int indice;
/* int linee = 1;   serve per mantenere il numero corrente della linea e e' stata usata solo nelle stampe di controllo   */

if (argc < 4)
{
	printf("Errore numero di parametri\n");
	exit(1);
}

/* conversione ultimo parametro: numero di linee dei file */
X = atoi(argv[argc-1]);
printf("Numero di linee %d\n", X);
if (X < 0)
{
	printf("Errore numero linee: non strettamente maggiore di zero\n");
	exit(2); 
}

/* numero di file e quindi numero di processi da creare */
N = argc-2;
printf("Numero di processi da creare %d\n", N);

/* allocazione primiCarNumi come array di caratteri: lo fa il padre e tutti i figli lo otterranno per copia */
if ((primiCarNum=(char *)malloc(X*sizeof(char))) == NULL)
{
	printf("Errore allocazione primiCarNum\n");
	exit(3); 
}

/* allocazione pipe */
if ((pipes=(pipe_t *)malloc(N*sizeof(pipe_t))) == NULL)
{
	printf("Errore allocazione pipe\n");
	exit(4); 
}

/* creazione pipe */
for (i=0;i<N;i++)
	if(pipe(pipes[i])<0)
	{
		printf("Errore creazione pipe\n");
		exit(5);
	}

/* creazione figli */
for (i=0;i<N;i++)
{
	if ((pid=fork())<0)
	{
		printf("Errore creazione figli\n");
		exit(6);
	}
	if (pid==0)
	{ /* codice figlio */
	printf("Sono il figlio %d\n", getpid());
	/* chiusura pipes inutilizzate */
	for (j=0;j<N;j++)
	{
		close (pipes[j][0]);
		if (j!=i)
			close (pipes[j][1]);
	}
 
	/* apertura file */
	if ((fd=open(argv[i+1],O_RDONLY))<0)
	{
		printf("Impossibile aprire il file %s\n", argv[i+1]);
		exit(-1);
	}
	/* j lo usiamo come indice nell'array primiCarNum */
	j = 0;
	/* inizializziamo primo a TRUE */
	primo = TRUE;
	while(read(fd,&ch,1)>0)
	{
		if (isdigit(ch) && primo) /* se e' il primo carattere numerico della linea allora lo salviamo nell'array */
  		{
            /*per salvare il carattere trovato dentro l'array*/
			primiCarNum[j] = ch;
			primo = FALSE;
			/* stampa di controllo  */
     		/*	printf("processo di indice %d ha salvato il carattere %c per la linea %d\n",i, ch, linee); */
	  	}
	
		/* controllo fine linea */
		if (ch == '\n')  /* siamo a fine linea */
     		{
			/* linee++; introdotto solo per stampe di controllo */
			primo = TRUE;
			j++; /*se siamo alla fine della linea, incrementiamo j per continuare a inserire gli altri primi caratteri numerici trovati nelle seguente linee*/
		 }
	}
	/* comunicazione primiCarNum al padre */
			nw=write(pipes[i][1], primiCarNum, X);
			if (nw!=X) 
			{ 
				printf("Errore in scrittura da pipe[%d]\n",i);
				exit(-1);
		 	}
			/* else printf("OK scrittura a padre da parte di processo %d\n", i); */

	exit(primiCarNum[X-1]); /* si deve trobare l'ultimo carattere numerico letto dal file */
	}
} /* fine for */

/* codice del padre */
/* chiusura pipe: tutte quelle in scrittura */
for (i=0;i<N;i++)
{
	close (pipes[i][1]);
}
/* inizializziamo max e indice per il calcolo del carattere numerico massimo e del figlio che lo ha inviato */
max=-1;
indice=-1;
/* il padre deve leggere N array di X caratteri, tanti quante sono le linee lette dai figli */
for (i=0;i<N;i++)
{
	nr=read(pipes[i][0], primiCarNum, X);
	if (nr!=X)
                        {
                                printf("Errore in lettura da pipe[%d]\n",i);
                                exit(7);
                        }
        printf("Ricevuto dal processo di indice %d per il file %s i seguenti primi caratteri numerici\n", i, argv[i+1]);
	for (j=0;j<X;j++)
        {
		printf("per la linea %d il carattere numerico %c\n", j+1,  primiCarNum[j]); /* incrementiamo j in modo da rendere piu' chiaro a che linea ci stiamo riferendo */
/* calcoliamo il carattere numerico massimo ricevuto su tutti gli array ricevuti */
 		if (primiCarNum[j] > max)
  		{
			max = primiCarNum[j];
			indice = i;
		} 
	} 
} 
/* stampiamo il valore intero corrispondente al carattere numerico massimo (facendo la opportuna conversione), insieme all'indice del processo che lo ha inviato */
printf("Il figlio di indice %d ha trovato il valore numerico massimo pari a %d\n", indice, (char)(max - '0'));
/* Il padre aspetta i figli */
for (i=0; i < N; i++)
        {
        pidFiglio = wait(&status);
        if (pidFiglio < 0)
                {
                	printf("Errore in wait\n");
                	exit(8);
                }

        if ((status & 0xFF) != 0)
                printf("Figlio con pid %d terminato in modo anomalo\n", pidFiglio);
        else
                { ritorno=(status >> 8) & 0xFF;
                  printf("Il figlio con pid=%d ha ritornato %c (in decimale %d se 255 errore)\n", pidFiglio, ritorno, ritorno);
		  /* N.B: Il valore tornato dai figli e' un carattere e quindi va stampato come tale */
                }
        }
return(0);
}