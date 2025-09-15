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
    int N; /*numero di file passati sulla riga di comando: i processi figli saranno il doppipo*/
     int pid; /*pid per fork*/
     pipe_t *pipe_f; /*array di pipe per la communicazione dai primi N figli agli ultimi N figli*/
     pipe_t *pipe_fbis; /*array di pipe per la communicazione dagli ultimi N figli ai primi N figli*/

     /*OSSERVAZIONE: creare due array separati rende più facile il calcolo dell'indice giusto da usare*/
     int fd; /*variabile per la open*/
     char ch; /*variabile da leggere dai figli*/
     char Cz; /*variabile per tenere traccia del carattere da cercare*/
     int occ; /*variabile per tenere traccia del numero di occorenze trovate*/
     long int pos; /*posizione del carattere trovato: inviamo il valore ricavato dalla lseek decrementato
     di 1 dato che dopo la lettura l'I/0 pointer è posizionato sul carattere seguente quello letto: quindi
     si considera che il primo carattere di ogni file sia posizione 0L*/
     long int posLetta; /*posizione corrente del carattere trovato ricevuta*/
    int pidFiglio, status, ritorno; /*variabili per wait*/
    int i,j; /*per i cicli*/
    int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/

     /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
     if(argc < 4)
     {
        printf("Errore sul numero di parametri: ci vuole almeno 3 parametri cioè almeno 2 nomi di file e un carattere\n");
        exit(1);
     }

     /*---------CALCOLARE IL VALORE DI N-----------------*/
     N = argc - 2; /*bisogna togliere il nome dell'eseguibile e l'ultimo parametro che è un carattere*/

     /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/
     /*controlliamo che l'ultimo parametro sia un singolo carattere*/

     if(strlen(argv[argc - 1]) != 1)
    {
        printf("Errore: l'ultimo parametro %s non è un singolo carattere\n", argv[argc-1]);
        exit(2);
    }
    /*isoliamo il singolo carattere*/
    Cz = argv[argc-1][0];

    printf("DEBUG-Sono il padre con pid %d e creo %d figli che dovranno cercare il carattere '%c\n", getpid(), N, Cz);

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    /*allocazione memoria dinamica per pipe_f e pipe_fbis*/
    pipe_f = malloc(N * sizeof(pipe_t));
    pipe_fbis = malloc(N * sizeof(pipe_t));
    if((pipe_f == NULL) || (pipe_fbis == NULL))
    {
        printf("Errore nella malloc\n");
        exit(3);
    }

    /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
    for(i=0; i< N; i++)
    {
        if(pipe(pipe_f[i]) != 0)
        {
            printf("Errore creazione della pipe %d-esima fra primi N figli e gli ultimi N\n",i);
            exit(4);
        }
        if(pipe(pipe_fbis[i]) != 0)
        {
            printf("Errore creazione della pipe %d-esima fra ultimi N figli e i primi N\n", i);
            exit(5);
        }
    }
    
    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), 2*N);

     /*ciclo di generazione dei figli*/
     for(i=0; i< 2*N ;i++)
     {
        if(pid = fork() < 0)
        {
            printf("Errore nella fork con indice %d\n", i);
            exit(6);
        }
        
        if(pid == 0)
        {
            /*codice del figlio Pi*/
            if(i < N) /*siamo nel codice dei primi N figli*/
            {
                printf("DEBUG-Figlio di indice %d e pid %d associato al file %s\n", i, getpid(), argv[i+1]);
                /*chiudiamo le pipe che non servono*/
                /*ogni figlio scrive solo su pipe_f[i] e legge solo da pipe_fbis[i]*/
                for( j=0; j<N; j++)
                {
                    close(pipe_f[j][0]);
                    close(pipe_fbis[j][1]);
                    if(j!=i)
                    {
                        close(pipe_f[j][1]);
                        close(pipe_fbis[j][0]);
                    }
                }

                /*per i primi N processi , il file viene individuato come al solito*/
                if((fd = open(argv[i+1], O_RDONLY)) < 0)
                {
                    printf("Errore: Impossibile aprire il file %s\n", argv[i+1]);
                    exit(0);
                }

                /*inizializziamo occ*/
                occ=0;

                /*leggiamo un carattere all volta*/
                while(read(fd, &ch, 1))
                {
                    if(ch == Cz) /*se abbimao trovato il carattere da cercare*/
                    {
                        /*incrementiamo occ*/
                        occ++;
                        /*calcoliamo la posizione del carattere*/
                        pos=lseek(fd,0L,SEEK_CUR) - 1L;
                        printf("DEBUG-VALORE di pos %ld per processo di indice %d che sto per mandare su pipe_f[i][1] %d\n", pos, i, pipe_f[i][1]);
                        /*inviamo la posizione del carattere all'altro processo della coppia*/
                        nw = write(pipe_f[i][1], &pos, sizeof(pos));
                        if(nw != sizeof(pos))
                        {
                            printf("Impossibile scrivere sulla pipe per il processo di indice %d\n", i);
                            exit(0);
                        }
                        /*aspettiamo dall'altro processo della coppia la nuova posizione da cui si deve riprendere la ricerca*/
                        nr = read(pipe_fbis[i][0], &posLetta, sizeof(posLetta));
                        if( nr != sizeof(posLetta))
                        {
                            /*se non viene inviata alcuna posizione vuole dire che l'altro processo della coppia NON ha trovato altre occorenze e quindi si può terminare la lettura*/
                            break;
                        }
                        printf("DEBUG-VALORE di pos %ls per processo di indice %d che ho ricevuto da pipe_fbis[i][0] %d\n", pos, i, pipe_fbis[i][0]);
                        /*spostiamo l'I/O pointer nella posizione seguente a quella ricevuta!*/
                        lseek(fd, posLetta+1L, SEEK_SET);
                    }
                    /*l'else non serve dato che NON si deve fare nulla e si deve continuare a leggere*/
                }
            }
            else /*siamo nel codice degli ultimi N figli*/
            {
                printf("DEBUG-SECONDA SERIE DI FIGLIO-Figlio di indice %d e pid %d assiciato al file %s\n", i, getpid(), argv[2*N-i]);
                /*SI PONE PARTICOLARE ATTENZIONE ALL'INDICE CHE SI DEVE USARE*/

                /*chiudiamo le pipe che non servono*/
                /*ogni figlio scrive solo su pipe_fbis[i] e legge solo da pipe_f[i]*/
                for( j=0; j < N; j++)
                {
                    close(pipe_f[j][1]);
                    close(pipe_fbis[j][0]);
                    if(j != 2*N-i-1)
                    {
                        close(pipe_f[j][0]);
                        close(pipe_fbis[j][1]);
                    }
                }
            }
            /*per gli ultimi N processi, il file viene individuato come indicato nel testo*/
            if((fd = open(argv[2*N-i], O_RDONLY)) < 0)
            {
                printf("Impossibile aprire il file %s\n", argv[2*N-i]);
                exit(0);
            }
            /*inizializziamo occ*/
            occ=0;
            /*per prima cosa dobbiamo aspettare la posizione dell'altro figlio*/
            nr=read(pipe_f[2*N-i-1][0], &posLetta, sizeof(posLetta));
            if(nr != sizeof(posLetta))
            {
                printf("Errore: Impossibile leggere dalla pipe per il processo di indice %d (PRIMA LETTERA)\n",i);
                exit(0);
            }
            printf("DEBUG-VALORE DI pos %ld per processo di indice %d che ho ricevuto da pipe_fbs[2*N-i-1][0] %d\n", pos, i, pipe_fbis[2*N-i-1][0]);
            /*spostiamo l'I/0 pointer nella posizione seguente a quella ricevuta!*/
            lseek(fd,posLetta+1L,SEEK_SET);
            /*quindi leggiamo un carattere alla volta*/
            while(read(fd,&ch,1))
            {
                if(ch == Cz) /*se abbiamo trovato il carattere da cercare*/
                {
                    /*incrementiamo occ*/
                    occ++;
                    /*calcoliamo la posizione del carattere*/
                    pos=lseek(fd,0L,SEEK_CUR) - 1L;
                    /*inviamo la posizione del carattere all'altro processo della coppia*/
                    nw = write(pipe_fbis[2*N-i-1][1], &pos, sizeof(pos));
                    if(nw != sizeof(pos))
                    {
                        printf("Errore: Impossibile scrivere sulla pipe per il processo di indice %d\n", i);
                        exit(0);
                    }
                    /*aspettiamo dall'altro processo della coppia la nuova posizione da cui si deve riprendere la ricerca*/
                    nr = read(pipe_f[2*N-i-1][0],&posLetta,sizeof(posLetta));
                    if(nr != sizeof(posLetta))
                    {
                        /*se non viene inviato alcuna posizione vuole dire che l'altro processo della coppia NON ha trovato altre occorenze e quindi si può terminare la lettura*/
                        break;
                    }
                    /*spostiamo l'I/O pointer nella posizione seguente a quella ricevuta!*/
                    lseek(fd,posLetta+1L,SEEK_SET);
                }
            }
            exit(occ); /*torniamo il numero di occorenze trovate */
        }
        
     }

     /*codice del padre*/
     /*chiudiamo tutte le pipe, dato che le usano solo i figli*/
     for(i=0; i<N; i++)
     {
        close(pipe_f[i][0]);
        close(pipe_f[i][1]);
        close(pipe_fbis[i][0]);
        close(pipe_fbis[i][1]);
     }

     /*attesa della terminazione dei figli*/
     for (i=0; i < 2*N; i++)
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
} /*fine del main*/