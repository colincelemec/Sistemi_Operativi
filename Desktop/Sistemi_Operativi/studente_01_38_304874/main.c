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
{
    /*---------VARIABILI LOCALI-------------------*/
    int pid; /*pid per fork*/
    int Y; /*numero di file passati sulla riga di comando*/
    int fd; /*variabile per la open*/
    char car; /*per il carattere corrente letto dai figli dal proprio file*/
    char carDispari='\0'; /*per leggere il carattere dispari dal file sul secondo processo*/
    char minCar='\0'; /*per calcolare il minimo carattere che dobbiamo ritornare al padre*/
    int y,j; /*per i cicli*/
    int CreatedFile; /*variabile per il file descriptor del file creato*/
    char *Fcreato; /*array di char per il file che vogliamo creare*/
    int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
    pipe_t *piped_f; /*array di pipe per la communicazione dai primi Y figli agli ultimi Y figli*/
  
    int count=0; /*per contare i caratteri trovati nel file*/
    int pidFiglio, status, ritorno; /*variabili per wait*/
    /*-----------------------------------------------*/


     /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
     if(argc < 2)
     {
        printf("Errore sul numero di parametri perché ci vuole almeno 1 file e il nome dell'eseguibile\n");
        exit(1);
     }

     /*---------CALCOLARE IL VALORE DI N-----------------*/
     Y = argc - 1; /*Ci togliamo il nome dell l'eseguibile*/

     /*CONTROLLO CHE I FILE HANNO LUNGHEZZA IN CARATTERI  PARI*/

     for(y = 0; y < argc ; y++)
     {
        if(strlen(argv[y+1]) % 2 != 0)
        {
            printf("Errore: La lunghezza in caratteri del file non è pari\n");
            exit(2);
        }
     }

     /*NEL CASO DI UN ARRAY DI 2*Y PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped_f = (pipe_t *)malloc(2 * Y * sizeof(pipe_t));
    if(piped_f == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe piped_f\n");
        exit(3);
    }

   

    /*CREAZIONE DI QUESTI Y PIPE ALLOCATI*/
    for(y=0; y < 2*Y; y++)
    {
        if(pipe(piped_f[y]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice y = %d\n", y);
            exit(4);
        }

       
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), 2*Y);

    /*ciclo di generazione dei figli*/
    for(y=0; y < 2*Y ;y++)
    {/*inizio ciclo for*/

        /*CREAZIONE DEI PROCESSI FIGLI*/
         if((pid = fork()) < 0)
            {
                printf("Errore nella fork di indice %d-esima\n", y);
                exit(6);
            }

        if(pid == 0)
        { /*inizio processo figlio*/

            /*NEL CASO DOVE ABBIMAO UN TIPO DI ESERCIZI CHE LAVORA CON 2*Y PROCESSI DOVE è DIVIVO IN DUE PROCESSI
                : IL PRIMO CHE VA DAL PROCESSO P0 A PY-1 E IL SECONDO CHE VA DAL PROCESSO PY AL PROCESSO P2*Y-1, ALLORA BISOGNA
                FARE UN IF PER SPECIFICARE LA PRIMA PARTE DEI PRIMI PROCESSI E UN ELSE PER GLI ALTRI N PROCESSI*/

            if(y < Y)
            { /*inizio primo processo figlio*/
                
                /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL LA PRIMA PARTE DEL PROCESSO FIGLIO*/
                printf("DEBUG-OK: Sono il primo processi figlio di indice  %d con pid %d e sono associato al file %s", y, getpid(), argv[y+1]);

                /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE */
                /*ogni figlio scrive solo su pipe_fbis[i] e legge solo da pipe_f[i]*/
                /*QUESTO PER LA PRIMA PARTE DEL PROCESSO (i < N)*/
                for( j=0; j<Y; j++)
                {
                    close(piped_f[j][0]);
                   
                    if(j!=y)
                    {
                        close(piped_f[j][1]);
                       ;
                    }
                }

                /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[y+1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[y+1]);
                    exit(-1);
                }

                /*per prima cosa, creare un file il cui nome risulti dalla concatenzaione del nome del 
                file associato alla coppia con la stringa .Min*/

                /*creaimo un file Fcreato i cui nome sia la concatenazione del nome del file associato Ff con la stringa ".Min"*/
                /*bisogna allocare una stringa lunga come il nome del file + il carattere '.' + i caratteri della parola Min(3) + IL TERMINTAORE DELLA STRINGA*/
                Fcreato = (char *)malloc(strlen(argv[y+1]) + 10);
                if(Fcreato == NULL)
                {
                    printf("Errore nella allocazione dinamica per la creazione del file %s\n",Fcreato);
                    exit(-1);
                }
                /*copiamo il nome del file associato al figlio*/
                strcpy(Fcreato,argv[y+1]);
                strcat(Fcreato,".Min");
                CreatedFile = creat(Fcreato, 0644);

                /*verifchimao che il file è stato ben creato*/
                if(CreatedFile < 0)
                {
                    printf("Errore nella creazione del file %s\n", Fcreato);
                    exit(-1);
                }

                /*il primo processo della coppia deve ricevere dal secondo il carattere inviato*/
                    nr = read(piped_f[y][0], &car, 1);

                    /*verifichiamo che la lettura sia andata a buon fine*/
                    if(nr != 1)
                    {
                        printf("Errore nella lettura da pipe piped_f\n");
                        exit(-1);
                    }

                /*allora, dobbiamo leggere tutti i caratteri del proprio file associato*/
                while(read(fd, &car, 1)) /*leggiamo un carattere all volta*/
                {   
                    /*incrementiamo count */
                    count++;
                    /*dobbiamo identificare i caratteri in posizione pari*/

                    if (count % 2 == 0) /*se abbiamo trovato un carattere in posizione pari*/
                    {
                        
                        /*dobbiamo scrivere sul proprio file creato il proprio carattere di pozione pari se questo
                        è minore di quello ricevuto */
                        if(car < carDispari)
                        {
                            /*scriviamo car cioè il carattere di posizione pari sul file creato*/
                            write(CreatedFile, &car, 1);
                        }else /*scriviamo il carattere di posizione dispari sul file creato */
                        {
                            write(CreatedFile, &carDispari, 1);
                        }
                        
                    }

                    
                }
                /*Il processo deve ritornara la padre il minimo calcolato sui caratteri di propria competenza*/
                if(car < minCar)
                {
                    minCar = car;
                }

                exit(minCar);
                 
            }/*fine primo processo figlio*/
            else
            {/*inizio secondo processo figlio*/
                /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL LA SECONDA PARTE DEL PROCESSO FIGLIO*/
                printf("DEBUG-OK: Sono il secondo processo figlio di indice  %d con pid %d e sono associato al file %s", Y+y, getpid(), argv[Y+y+1]);
                /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE */
                /*QUESTO PER LA SECONDA PARTE (else)*/
                for( j=0; j < Y; j++)
                {
                    close(piped_f[j][1]);
                    
                    if(j != 2*Y-j-1)
                    {
                        close(piped_f[j][0]);
                        
                    }
                }

                /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[Y+y+1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[Y+y+1]);
                    exit(-1);
                }

                /*dobbiamo anche leggere tutti i caratteri dal file file associato*/
                while(read(fd, &carDispari, 1)) /*leggiamo un carattere alla volta*/
                {
                    /*incrementiamo il conteggio*/
                    count++;
                    
                    /*dobbiamo identificare i caratteri in posizione dispari*/
                    if(count % 2 != 0) /*se abbiamo trovato un file in posizione dispari*/
                    {
                        /*dobbiamo mandare il carattere trovato al primo processo della coppia*/
                        nw = write(piped_f[y][1], &carDispari, 1);
                        /*controlliamo se il carattere è stato ben mandato*/
                        if(nw != 1)
                        {
                            printf("Errore nella scrittura sulla pipe pipe_f\n");
                            exit(-1);
                        }
                    }
                }

                /*Il processo deve ritornara la padre il minimo calcolato sui caratteri di propria competenza*/
                if(carDispari < minCar)
                {
                    minCar = carDispari;
                }

                exit(minCar);

            }/*fine secondo processo figlio*/
            
        }/*fine processo figlio*/

    } /*fine ciclo for*/

    /*padre*/
    /*chiude il lato della pipe che non usa*/

    /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (y=0; y < Y ; ++y)
    {
        close(piped_f[y][1]);
    }

    /*attesa della terminazione dei figli*/
    for (y=0; y < Y; y++)
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