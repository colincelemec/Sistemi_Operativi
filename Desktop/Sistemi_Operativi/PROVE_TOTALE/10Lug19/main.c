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
    int N; /*numero di file passati sulla riga di comando*/
    int fd; /*variabile per la open*/
    int i,j; /*per i cicli*/
     int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
     char c; /*per leggere dal file*/
    char Cz; /*carattere da controllare per ogni file*/
     long int pos1; /*VARIABILE CHE PUO ESSERE USATO PER COMMUNICARE LA POSIZIONE DEL PRIMO PROCESSO A SECONDO */
     long int pos2; /*VARIABILE CHE PUO ESSERE USATO PER COMMUNICARE LA POSIZIONE DEL SECONDO PROCESSO AL PRIMO */
    pipe_t *piped_f; /*array di pipe per la communicazione dai primi N figli agli ultimi N figli*/
    pipe_t *piped_fbis; /*array di pipe per la communicazione dagli ultimi N figli ai primi N figli*/
    int cont_occ = 0; /*per il numero di occorenze del carattere nel secondo processo*/
    int pidFiglio, status, ritorno; /*variabili per wait*/
   
    /*-----------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/
    if(argc < 4)
    {
        printf("Errore sul numero di parametri. Ci vuole almeno 3 parametri!");
        exit(1);
    }

    /*---------CALCOLARE IL VALORE DI N-----------------*/
    N = argc - 2; /*perché si deve togliere l'ultimo parametro e il nome dell'eseguibile */

    /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/

    /*Controllo che sia un singolo carattere*/
    if(strlen(argv[argc-1]) != 1)
    {
        printf("Errore: l'ultimo %s non è un singolo carattere\n", argv[argc-1]);
        exit(2);
    }
    /*Isoliamo il singolo carattere*/
        Cz = argv[argc-1][0];

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped_f = (pipe_t *)malloc(N * sizeof(pipe_t));
    if(piped_f == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe del primo processo\n");
        exit(3);
    }

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped_fbis = (pipe_t *)malloc(N * sizeof(pipe_t));
    if(piped_fbis == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe del secondo processo\n");
        exit(4);
    }

    /*CREAZIONE DI QUESTI 2*N PIPE ALLOCATI*/
    for(i=0; i< N; i++)
    {
        if(pipe(piped_f[i]) < 0 || pipe(piped_fbis[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(4);
        }
    }

    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), 2*N);

    /*ciclo di generazione dei figli*/
     for(i=0; i<2*N ;i++)
     {/*inizio ciclo for*/

        /*CREAZIONE DEI PROCESSI FIGLI*/
         if((pid = fork()) < 0)
            {
                printf("Errore nella fork di indice %d-esima\n", i);
                exit(5);
            }
        if(pid == 0)
        { /*inizo figlio*/
            /*NEL CASO DOVE ABBIMAO UN TIPO DI ESERCIZI CHE LAVORA CON 2*N PROCESSI DOVE è DIVIVO IN DUE PROCESSI
                : IL PRIMO CHE VA DAL PROCESSO P0 A PN E IL SECONDO CHE VA DAL PROCESSO PN AL PROCESSO PN-1, ALLORA BISOGNA
                FARE UN IF PER SPECIFICARE LA PRIMA PARTE DEI PRIMI PROCESSI E UN ELSE PER GLI ALTRI N PROCESSI*/

            if ( i < N)
            { /*prima parte de processo*/
                
                /*NEL CASO DOVE ABBIAMO 2*N PROCESSI E VOGLIAMO LAVORARE SU DUE PARTE DISTINTI CON 2 ARRAY DI PIPE ALLOCATI PER LO SCHEMA DI COMMUNICAZIONE*/
               /*chiudiamo le pipe che non servono*/
                /*ogni figlio scrive solo su piped_fbis[i] e legge solo da piped_f[i]*/
                for( j=0; j<N; j++)
                {
                    close(piped_f[j][0]);
                    close(piped_fbis[j][1]);
                    if(j!=i)
                    {
                        close(piped_f[j][1]);
                        close(piped_fbis[j][0]);
                    }
                }

                /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PRIMO PROCESSO FIGLIO*/
                /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[i+1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[i+1]);
                    exit(-1);
                }
                printf("DEBUG-Sono il primo processo figlio con di indice %d con pid %d e sono accociato al file %s e sto per cercare il carattere %c\n", i,getpid(), argv[i+1], Cz );

                /*prima di tutto , cerchiamo il carattere Cz dentro il file associato*/

                while(read(fd, &c, 1)) /*leggiamo un carattere alla volta il file*/
                {
                    if(c == Cz) /*se abbiamo trovato il carattere */
                    {
                        /*incrementiamo il numero di occorenze*/
                        cont_occ++;
                        /*calcoliamo la posizione di questo carattere*/
                        pos1 = lseek(fd, 0L, SEEK_CUR)-1L;
                        printf("DEBUG-Valore di pos %ld per processo di indice %d che sto per mandare su piped_f[i][1] %d", pos1, i, piped_f[i][1]);
                        /*Appena trovato il carattere, communichiamo la sua posizione al secondo processo della coppia*/
                        nw = write(piped_f[i][1], &pos1, sizeof(pos1));
                        /*verifichiamo che la posizone è stato scritto correttamente*/
                        if(nw != sizeof(pos1))
                        {
                            printf("Errore nella scrittura\n");
                            exit(-1);
                        }

                        /*ricezione della posizione dal secondo processo*/
                        nr = read(piped_fbis[i][0], &pos2, sizeof(pos2));
                       
                        /*verifichiamo che il la poszione è stat letta correttamente*/
                        if( nr != sizeof(pos2))
                        {
                            printf("Errore nella lettura della posizione\n");
                            exit(-1);
                        }
                         printf("DEBUG-VALORE di pos %ld per processo di indice %d che ho ricevuto da piped_fbis[i][0] %d\n", pos2, i, piped_fbis[i][0]);
                         /*spostiamo l'I/O pointer nella posizione seguente a quella ricevuta!*/
                         lseek(fd, pos2+1L, SEEK_SET);
                    }
                }
                
            } /*fine parte del primo ciclo*/
            else{ /*inizio parte del secndo ciclo*/
                /*chiudiamo le pipe che non servono*/
                /*ogni figlio scrive solo su pipe_fbis[i] e legge solo da pipe_f[i]*/

                for( j=0; j < N; j++)
                {
                    close(piped_f[j][1]);
                    close(piped_fbis[j][0]);
                    if(j != 2*N-i-1)
                    {
                        close(piped_f[j][0]);
                        close(piped_fbis[j][1]);
                    }
                }

                /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PRIMO PROCESSO FIGLIO*/
                /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[2*N - i], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[2*N - i]);
                    exit(-1);
                }
                printf("DEBUG-sono il secondo processo figlio con pid %d di indice %d, sono associato al file %s e sto per cercare il carattere %c\n", getpid(), i , argv[2*N-i], Cz);

                /*ricezione della posizione inviata dal primo processo*/
                nr = read(piped_f[2*N-i-1][0], &pos2, sizeof(pos2));
                if(nr != sizeof(pos2))
                {
                    printf("Errore nella lettura della poszione\n");
                    exit(-1);
                }


                while(read(fd, &c, 1)) /*leggiamo un carattere alla volta*/
                {
                    if(c == Cz) /*se abbiamo trovato il carattere*/
                    {
                        /*incrementiamo il numero di occorenze*/
                        cont_occ++;
                        /*appena trovato una occorenza del carattere nel file, */
                        /*communichiamo al primo processo della coppia la posizone del carattere*/
                        /*calcoliamo la posizione del carattere*/
                        pos2 = lseek(fd, 0L, SEEK_CUR)-1L;

                        /*mandiamolo al primo processo*/
                        nw = write(piped_f[2*N-i-1][1], &pos2, sizeof(pos2));
                        /*verifichiamo che è stato scritto bene*/
                        if( nw != sizeof(pos2))
                        {
                            printf("Errore nella scrittura\n");
                            exit(-1);
                        }
                    }
                }

                


            } /*fine parte del secondo ciclo*/
            exit(cont_occ);
        } /*fine figlio*/

     } /*fine ciclo for*/

     /*padre*/
    /*chiude il lato della pipe che non usa*/

    /*NEL CASO DOVE ABBIAMO 2*N PROCESSI E VOGLIAMO LAVORARE SU DUE PARTE DISTINTI CON 2 ARRAY DI PIPE ALLOCATI PER LO SCHEMA DI COMMUNICAZIONE*/
               /*chiudiamo le pipe che non servono*/
    /*chiudiamo tutte le pipe, dato che le usano solo i figli*/

    for(i=0; i<N; i++)
     {
        close(piped_f[i][0]);
        close(piped_f[i][1]);
        close(piped_fbis[i][0]);
        close(piped_fbis[i][1]);
     }

      /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PADRE*/

      /*ogni processo figlio deve ritornare al padre il numero di occorenze del carattere Cz trovate dal singolo processo*/

      printf("Il primo processo ha ritornato %d occorenze del carattere %c e il secondo processo ne ha ritornato %d", cont_occ,Cz, cont_occ);


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
    
}