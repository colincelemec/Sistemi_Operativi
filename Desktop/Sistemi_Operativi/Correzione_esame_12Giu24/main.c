

/* Versione funzionante della parte C dell'esame del 12 Giugno 2024
 la comunicazione in ogni coppia va dal primo processo della coppia al secondo processo della coppia ed e' 
 il secondo processo della coppia deve creare il file con terminazione ".min" sul quale poi deve scrivere */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#define PERM 0644

typedef int pipe_t[2];

int main(int argc, char **argv) 
{
	/* -------- Variabili locali ---------- */
	int pid;			/* process identifier per le fork() */
	int V; 				/* numero di file passati sulla riga di comando */
	int lung; 			/* minimo delle lunghezze delle linee calcolate da ogni figlio */
	int nroLinea; 			/* numero delle linee calcolate da ogni figlio e dal padre */
	char *FCreato;          	/* variabile per nome file da creare da parte dei processi figli secondi della coppia */

	int createdfile; 			/* file descriptor per creazione file da parte dein processi figli secondi della coppia */
	int fd; 			/* file descriptor per apertura file */	
	pipe_t *pipe_ps;       	 	/* array di pipe per la comunicazione dai figli primi della coppia ai figli secondi della coppia */
	int v;				/* indice per i figli */
	int i, j;			/* indici per i cicli */
	char BUFF[250];		/* array di caratteri per memorizzare la linea: come indicato dal testo si puo' supporre una lunghezza massima di ogni linea di 250 caratteri compreso il terminatore di linea */
	int jLetto; 			/* variabile usata dal figlio secondo della coppia per recuperare le lunghezze inviate dal figlio primo della coppia */
	char lineaLetta[250];		/* variabile usata dal figlio secondo della coppia per recuperare le linee inviate dal figlio primo della coppia */
	int status, ritorno;		/* variabili per la wait */
	/* ------------------------------------ */

	/* Controllo sul numero di parametri */
	if (argc < 2) /* Meno di un parametro */  
	{
		printf("Errore nel numero dei parametri dato che argc=%d (bisogna passare almeno un nome di file!)\n", argc);
		exit(1);
	}

	/* Calcoliamo il numero di file passati */
	V = argc - 1;
	printf("Sono il padre con pid %d e %d file e creero' %d processi figli\n", getpid(), V, 2*V);
	
       	/* allocazione memoria dinamica per pipe_ps. */	       
	pipe_ps = (pipe_t *) malloc (V*sizeof(pipe_t));
	if (pipe_ps == NULL)
	{
		printf("Errore nella allocazione della memoria per le pipe\n");
		exit(2);
	}
	
	/* Creazione delle N pipe */
	for (i=0; i < V; i++)
	{
		if(pipe(pipe_ps[i]) < 0)
		{
			printf("Errore nella creazione della pipe\v");
			exit(3);
		}
	}

	/* Ciclo di generazione dei figli  */
	for (v=0; v < 2*V; v++)
	{
		if ( (pid = fork()) < 0)
		{
			printf("Errore nella fork del figlio %d-esimo\n", v);
			exit(4);
		}
		
		if (pid == 0) 
		{
			/* codice del figlio: in caso di errore torniamo -1 che non e' un valore accettabile*/
			if (v < V) /* siamo nel codice dei figli primi della coppia */
			{
				/* stampa di debugging */
				printf("DEBUG-PRIMO DELLA COPPIA-Figlio di indice %d e pid %d associato al file %s\n",v,getpid(),argv[v+1]); 
				/* chiudiamo le pipe che non servono */
				/* ogni figlio PRIMO della coppia scrive solo sulla pipe_ps[v] */
				for (j=0;j<V;j++)
				{
					close(pipe_ps[j][0]);
					if (j!=v)
					{
						close(pipe_ps[j][1]);
					}
				}
			 	/* ogni figlio deve aprire il suo file associato */
                                fd=open(argv[v+1], O_RDONLY);
                                if (fd < 0)
	                        {
		                	printf("Impossibile aprire il file %s\n", argv[v+1]);
			                exit(-1); /* in caso di errore, decidiamo di tornare -1  */ 
				}

				/* adesso il figlio legge dal file una linea alla volta */
				j=0; 		/* azzeriamo l'indice della linea */
				nroLinea=0; 	/* azzeriamo il numero della linea */
				lung=251;	/* settiamo il minimo a 251, che non e' un valore ammissibile rispetto alla lunghezza massima stabilita dal testo */
		        	while (read(fd, &(BUFF[j]), 1))
				{
					if (BUFF[j] == '\n') 
			 		{ 
						nroLinea++; /* la prima linea sara' la numero 1! */
						//printf("DEBUG-Figlio %d e pid %d nroLinea=%d\n", v, getpid(), nroLinea);
						if ((nroLinea %2) == 1) 
			 			{ 
							/* se siamo su una linea dispari */
							/* dobbiamo mandare al secondo processo della coppia la lunghezza della linea selezionata compreso il terminatore di linea (come int) e quindi incrementiamo j */
				   			j++;
							/* quindi, per prima cosa si invia la lunghezza della linea */
							write(pipe_ps[v][1], &j, sizeof(j));
							/* e poi anche la linea */
							write(pipe_ps[v][1], BUFF, j);

							/* verifichiamo e nel caso aggiorniamo il massimo */
							//printf("DEBUG-Figlio %d e pid %d j=%d e nro=%d\n", v, getpid(), j, nro);
							if (j < lung) 
								lung=j; 
						}
				   		j=0; /* azzeriamo l'indice per le prossime linee */
					}
					else j++; /* continuiamo a leggere */
				}
  			}	
			else /* siamo nel codice dei figli secondi della coppia */
			{
				/* stampa di debugging */
				printf("DEBUG-SECONDO DELLA COPPIA-Figlio di indice %d e pid %d associato al file %s\n",v,getpid(),argv[v-V+1]);

				/* i figli secondi della coppia devono creare il file specificato */
				FCreato=(char *)malloc(strlen(argv[v-V+1]) + 5); /* bisogna allocare una stringa lunga come il nome del file associato + il carattere '.' + i caratteri della parola min (3) + il terminatore di stringa 
				quindi aggiungeremo 5 alla lunghezza iniziale del file*/
				if (FCreato == NULL)
				{
					printf("Errore nella malloc\n");
					exit(-1);
				}
				/* copiamo il nome del file associato */
				strcpy(FCreato, argv[v-V+1]);
				/* concateniamo la stringa specificata dal testo */
				strcat(FCreato,".min");
				createdfile=creat(FCreato, PERM);
				if (createdfile < 0)
				{
					printf("Impossibile creare il file %s\n", FCreato);
					exit(-1);
				}

				/* chiudiamo le pipe che non servono */
				/* ogni figlio SECONDO della coppia legge solo da pipe_ps[v-V] */
				for (j=0;j<V;j++)
				{
					close(pipe_ps[j][1]);
					if (j!= v-V)     /* ATTENZIONE ALL'INDICE CHE DEVE ESSERE USATO */ 	
					{
						close(pipe_ps[j][0]);
					}
				} 
				/* ogni figlio deve aprire il suo file associato: siamo nei figli secondi della coppia e quindi attenzione all'indice */
				fd=open(argv[v-V+1], O_RDONLY);
				if (fd < 0)
				{
					printf("Impossibile aprire il file %s\n", argv[v-V+1]);
					exit(-1);
				}

				/* adesso il figlio legge dal file una linea alla volta */
				j=0; 		/* azzeriamo l'indice della linea */
				nroLinea=0; 	/* azzeriamo il numero della linea */
				lung=251;	/* settiamo il minimo a 251, che non e' un valore ammissibile rispetto alla lunghezza massima stabilita dal testo */
		        	while (read(fd, &(BUFF[j]), 1))
				{
					if (BUFF[j] == '\n') 
			 		{ 
						nroLinea++; /* la prima linea sara' la numero 1! */
						//printf("DEBUG-Figlio %d e pid %d nroLinea=%d\n", v, getpid(), nroLinea);
						if ((nroLinea %2) == 0) 
			 			{ 
							/* dato che dobbiamo ricevere dal primo processo della coppia la lunghezza della linea selezionata compreso il terminatore di linea (come int) e quindi incrementiamo j */
				   			j++;
							/* quindi, per prima cosa si legge la lunghezza della linea */
							read(pipe_ps[v-V][0], &jLetto, sizeof(jLetto));
							/* e poi anche la linea */
							read(pipe_ps[v-V][0], &lineaLetta, jLetto);
							//printf("DEBUG-Per Figlio %d e pid %d: j=%d e jLetto=%d\n", v, getpid(), j, jLetto);
							if (j < jLetto)
			 				{		/* se la linea del secondo figlio della coppia e' piu' corta, scriviamo quella sul file */
								write(createdfile, BUFF, j);
							}
							else
			 				{	 /* scriviamo la linea ricevuta sul file */
								write(createdfile, lineaLetta, jLetto);
							}

							/* verifichiamo e nel caso aggiorniamo il minimo */
							//printf("DEBUG-Figlio %d e pid %d j=%d e lung=%d\n", v, getpid(), j, lung);
							if (j < lung) 
								lung=j; 
						}
				   		j=0; /* azzeriamo l'indice per le prossime linee */
					}
					else j++; /* continuiamo a leggere */
				}
			}

			/* ogni figlio deve ritornare al padre il valore corrispondente al minimo */
			exit(lung);
		}
	}
	
	/* Codice del padre */
	/* Il padre chiude tutti i lati delle pipe dato che non le usa */
	for (i=0; i < V; i++)
 	{
		close(pipe_ps[i][0]);
		close(pipe_ps[i][1]);
	}

	/* Il padre aspetta tutti i figli */
	for (v=0; v < 2*V; v++)
	{
		pid = wait(&status);
		if (pid < 0)
		{
		printf("Errore in wait\n");
		exit(5);
		}

		if ((status & 0xFF) != 0)
    			printf("Figlio con pid %d terminato in modo anomalo\n", pid);
    		else
		{ 
			ritorno=(int)((status >> 8) &	0xFF); 
			printf("Il figlio con pid=%d ha ritornato %d (se 255 problemi)\n", pid, ritorno);
		}
	}

	exit(0);
}