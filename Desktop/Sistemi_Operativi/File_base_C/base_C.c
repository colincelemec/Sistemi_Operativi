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

/*definizione del tipo di data Struct*/
typedef struct  
{
    char c; /*carattere controllato*/
    long int n; /*numero di occorenze del carattere*/
} tipoS;

/*definizione del tipo di data Struct apposto per la logica di implementazione dello schema di communicazione a PIPELINE*/
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

/*On peut utiliser le 
while(!finito){
} lorqu'on veut recevoir les informations des fils par ordre pour vérifier si les 
    processus on bien étés terminés
*/

int mia_random(int n)
{
    int casuale;
    casuale = rand() % n; /*con rand() viene calcolato un numero pseudo casuale e con l'operazione 
                            modulo n si ricava un numero casuale compreson fra 0 e n-1*/
    return casuale;
}

int main (int argc , char **argv){

    /*---------VARIABILI LOCALI-------------------*/
    int pid; /*pid per fork*/
    /*Se la specifica indica che il padre deve stampare su stdout il pid dei figli una volta ricevuto l'array, allora si deve allocare un array di pid (int *pid)*/
     int N; /*numero di file passati sulla riga di comando*/
     int fd; /*variabile per la open*/
    int i,j; /*per i cicli*/
    int nr,nw; /*variabili per salvare valori di ritorno di read e write da/su pipe*/
    data d[N]; /*VARIABILE DI TIPO STRUCT PER LA NOSTRA LOGICA DI IMPLEMENTAZIONE DELLO SCHEMA DI COMMUNICAZIONE A PIPELINE.
                QUESTO É PER UN ARRAY STATICO , MA SE DOVESSIMO FARLO PER UN ARRAY DINAMICO , SAREBBE 'data *d' e poi avremo 
                dovuto fare una malloc per l'allocazione e eventualmente verificare se la malloc è andata a buon fine.*/

    long int cont; /*per conteggio*/
    long int pos; /*VARIABILE CHE PUO ESSERE USATO PER COMMUNICARE LA POSIZIONE (SI PUO USARE QUANDO NEL TESTO SI DICE "PER OGNI POSIZIONE RICEVUTA")*/
    /*IN CASO DI ESAME SAREBBE ME*/

    /*NEL CASO DOVE VOGLIAMO SELEZIONARE UNA LINEA , ECCO GLI EVENTUALI VARIABILI CHE POSSIAMO USARE*/
    char buffer[255]; /*per leggere i caratteri dal file, supponendo che una linea non sia più lunga di 255 compreso il terminatore di linea ed il terminatore di stringa perché trasformeremo la linea in una stringa per poterla stampare con printf*/
    int trovata=0; /*indica se è stata trovata la linea n-esima: usata come un booleano*/
     int  n; /*n serve per contare le linee, n intero corrispondente al secondo parametro*/
     pipe_t *piped; /*array dinamico di pipe per communicazioni figli-padre*/
     pipe_t p; /*singola pipe per communicazione figlio-nipote*/
     int pidFiglio, status, ritorno; /*variabili per wait*/
        /*PER LA STRUCT*/
    char c; /*per leggere dal file*/
    char C; /*carattere da controllare per ogni file*/

    /*NEL CASO DOVE VOGLIAMO LAVORARE CON UNO SCHEMA DI COMMUNICAZIONE CHE CONTIENE  DUE ARRAY DI PIPE , ALLORA POTREMO DEFINIRE QUESTI VARIABILI*/
    pipe_t *pipe_f; /*array di pipe per la communicazione dai primi N figli agli ultimi N figli*/
    pipe_t *pipe_fbis; /*array di pipe per la communicazione dagli ultimi N figli ai primi N figli*/
    
    tipoS msg; /*struttura per la communicazione da figli a padre*/
        /*NEL CASO DOVE ABBIAMO UN ARRAY DI PIPE PER CHIUDERE LE PIPE NON UTILIZZATE*/
   
    /*-----------------------------------------------*/

    /*--------CONTROLLO SUL NUMERO DI PARAMETRI-------------*/

    /*---------CALCOLARE IL VALORE DI N-----------------*/

    /*----------EVENTUALI CONTROLLO SUL TIPO DI PARAMETRI-----------*/

         /*controllo  che deve essere  il nome di un file andiamo a aprovare ad aprirlo*/
    if((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore: File %s NON ESISTE\n", argv[1]);
        exit(2);
    }

        /*Controllo che sia un numero intero strettamente positivo*/
        /*prima ,convertiamolo in un numero*/
    N=atoi(argv[2]);
    if(N <= 0){
        printf("Errore; il secondo parametro %s non è un numero strettamente maggiore di 0\n", argv[2]);
        exit(3);
    }

        /*Controllo che sia un singolo carattere*/
    if(strlen(argv[3]) != 1)
    {
        printf("Errore: il terzo parametro %s non è un singolo carattere\n", argv[3]);
        exit(4);
    }

        /*Isoliamo il singolo carattere*/
        c = argv[3][0];

        /*PER FARE UN CONTROLLO SU PIU PARAMETRI BASTA FARE ESEGUIRE UN CICLO*/

    /*NEL CASO DELLA SINGOLA PIPE*/
     /*creazione della singola pipe*/
    if(pipe(p) < 0)
    {
        printf("Errore nella creazione della pipe\n");
        exit(3);
    }

    /*NEL CASO DI UN ARRAY DI N PIPE DOBBIAMO FARE UNA ALLOCAZIONE DINAMICA*/
    piped = (pipe_t *)malloc(N * sizeof(pipe_t));
    if(piped == NULL)
    {
        printf("Errore nella allocazione dinamico per le pipe\n");
        exit(3);
    }

    /*CREAZIONE DI QUESTI N PIPE ALLOCATI*/
    for(i=0; i< N; i++)
    {
        if(pipe(piped[i]) < 0)
        {
            printf("Errore nella creazione delle pipe di indice i = %d\n", i);
            exit(4);
        }
    }

   
    /*SCRIVIAMO SU STDOUT IL PID DEL PADRE E IL NUMERO DI PROCESSI FIGLI DA GENERARE*/
    printf("DEBUG-Sono il processore padre con pid %d e sto per generare %d figli\n", getpid(), N);

       /*ciclo di generazione dei figli*/
         for(i=0; i<N ;i++)
        { /*inizio ciclo for*/
        /*CREAZIONE DEI PROCESSI FIGLI*/
            if((pid = fork()) < 0)
            {
                printf("Errore nella fork di indice %d-esima\n", i);
                exit(5);
            }

            if(pid == 0) 
            {

                /*NEL CASO DOVE ABBIMAO UN TIPO DI ESERCIZI CHE LAVORA CON 2*N PROCESSI DOVE è DIVIVO IN DUE PROCESSI
                : IL PRIMO CHE VA DAL PROCESSO P0 A PN E IL SECONDO CHE VA DAL PROCESSO PN AL PROCESSO PN-1, ALLORA BISOGNA
                FARE UN IF PER SPECIFICARE LA PRIMA PARTE DEI PRIMI PROCESSI E UN ELSE PER GLI ALTRI N PROCESSI 
                if(i < N){
                 scrivere il solito codice 
                }
                else
                {
                scrivere l'altra parte del codice
                }*/

                    /*codice del figlio Pi*/
                /*FARE UNA PRINTF PER DIRE COSA DEVE FARE IL PROCESSO FIGLIO*/

                /*CHIUSURA DELLE LATI DEI PIPE NON UTILIZZATE NELLA COMMUNICAZIONE CON IL PADRE*/
                /*NEL CASO DI UNA SINGOLA PIPE*/
                 close(p[0]);

                 /*NEL CASO DI N PIPE*/
                 for( j=0; j < N; j++)
                {
                /*ogni figlio NON legge da nessuna pipe e scrive solo sulla sua che è la n-esima!*/
                close(piped[j][0]);
                if(i != j)
                    close(piped[j][1]);
               }

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
               /*NEL CASO DI UNO SCHEMA DI COMMUNICAZIONE A RING*/
               /* chiusura  dei lati delle pipe non usate nella sincronizzazione */
			/* per capire le chiusure, fare riferimento al commento riportato per l'array pipes */
			for (j=0;j<N+1;j++)
			{	
				if (j!=i)
					close (piped[j][0]);
				if (j != (i+1))
					close (piped[j][1]);
			}
	

               /*NEL CASO DOVE ABBIAMO 2*N PROCESSI E VOGLIAMO LAVORARE SU DUE PARTE DISTINTI CON 2 ARRAY DI PIPE ALLOCATI PER LO SCHEMA DI COMMUNICAZIONE*/
               /*chiudiamo le pipe che non servono*/
                /*ogni figlio scrive solo su pipe_fbis[i] e legge solo da pipe_f[i]*/
                /*QUESTO PER LA PRIMA PARTE DEL PROCESSO (i < N)*/
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
                /*QUESTO PER LA SECONDA PARTE (else)*/
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


               /*LOGICA DI IMPLEMENTAZIONE DI UNO SCHEMA DI COMMUNICAZIONE A PIPELINE*/
               /*----------------------------------------------------------------------------------------------------------------*/
               /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[1]);
                    exit(-1);
                }

                cont = 0L; /*inizializziamo il conteggio*/
                /*ogni figlio deve leggere il proprio file , un carattere alla volta*/
                while(read(fd,&c,1))
                {
                    /*cerchiamo il carattere*/
                    if(c==C)
                        cont++; /*trovato il carattere e quindi si incrementa il conteggio*/
                }

                /*lettura da pipe dal figlio precedente dell'array di strutture per tutti i figli a parte il primo*/
                if(i!=0)
                {
                    nr=read(piped[i-1][0],d,sizeof(d));
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
                d[i].occ = cont;

                /*tutti i figli mandano in avanti (cioè al figlio successivo, a parte l'ultimo che manda al padre) un array di strutture di dimensione fissa*/
                nw=write(piped[i][1], d, sizeof(d));
                /*verifichiamo che la scrittura sia andata a buon fine*/
                if(nw != sizeof(d))
                {
                    printf("Errore in scrittura da pipe[%d]\n", i);
                    exit(-1);
                }
                /*torniamo l'ultimo carattere letto (che sarà chiaramente uguale per tutti i figli)*/
                exit(c);
                /*-----------------------------------------------------------------------------------------------------------------------*/

               /*LOGICA DI IMPLEMENTAZIONE DEL NIPOTE CHE ESEGUE CONCORRENTEMENTE CALCOLANDO LA LUNGHEZZA IN LINEE DEL FILE ASSOCIATO USANDO IN MODO OPPORTUNO IL COMANDO-FILTRO WC DI UNIX-LINUX */
              /*---------------------------------------------------------------------------------------------------------------------------*/
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
                    /*CODICE NIPOTE*/
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
                    /*Usiamo perror che scrive su stdout error, dato che lo standard output è collegato alla pipe*/
                    perror("Problemi di esecuzione del comando...da parte del nipote\n");

                    exit(-1);
                }
                /*codice figlio*/
                /*ogni figlio deve chiudere il lato che non usa della pipe di communicazione con il nipote*/
                close(p[1]);

                

                /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PROCESSO FIGLIO*/
              
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
                /*-------------------------------------------------------------------------------------------------------------*/
            

                 /*PEZZO DI CODICE PER SELEZIONARE UNA LINEA */
    /*-----------------------------------------------------------------------------------------------------------*/
     i = 1; /*inizializziamo il conteggio delle linee: la prima linea è la linea numero 1*/
    j = 0; /*valore iniziale dell'indice del buffer*/
    while(read(fd, &(buffer[j]), 1) != 0){ /*leggiamo un carattere alla volta e, contestualmente , lo inseriamo nell'aray buffer*/
        if(buffer[j] == '\n'){ /*se il carattere corrente è il terminatore di linea...*/
            if( n == i){ /*controlliamo se siamo sulla linea che dobbiamo selezionare*/
                /*dobbiamo prima rendere la linea una stringa*/
                buffer[j+1] = '\0';
                /*poi la stampiamo su stdout*/
                printf("la linea numero %d del file %s è: \n%s", n, argv[1], buffer);
                trovata=1; /*settiamo la variabile, dato che abbiamo trovato la linea cercata*/
                break; /*usciamo dal ciclo di lettura*/
            }else{ /*se non siamo sulla linea giusta*/
                j = 0; /*azzeriamo l'indice per la prossima linea*/
                i++; /*incrementiamo il conteggio delle linee*/
            }
        } 
        else j++; /*altrimenti incrementiamo l'indice nel buffer perché vuole dire che non siamo alla fine della linea*/
    }
    if(!trovata){ /*se non abbiamo trovato la linea richiesta*/
        printf("la linea numero %d non esiste in %s\n", n, argv[1]);
    }
    /*--------------------------------------------------------------------------------------------------------------*/

    /*PEZZO DI CODICE PER SELEZIONARE LA LUNGHEZZA DI UNA LINEA*/

    /*--------------------------------------------------------------------------------------------------------------*/
     i=1; /*inizializziamo il conteggio delle linee: la prima linea è la linea numero 1*/
    j=0; /*valore iniziale dell'indice del buffer*/
    while(read(fd, &(buffer[j]), 1) != 0){ /*leggiamo un carattere alla volta e lo inseriamo nell'array buffer*/
        if(buffer[j] == '\n'){ /*se il carattere corrente è il terminatore della linea*/
            if(n == j+1) /*controlliamo se la lunghezza della linea corrente ha la lunghezza giusta e quindi la stampiamo dopo averla resa una stringa. N.B: in questo caso non va inserito una break perché potremo chiaramente trovare piu linee della lunghezza cercata*/
            {
                buffer[j+1] = '\0'; /*dobbiamo prima rendere la linea una stringa*/
                printf("la linea numero %d del file %s ha la lunghezza cercata cioè %d:\n%s", i, argv[1], n, buffer);
                trovata=1; /*setttiamo la variabile , dato che abbiamo trovata almeno una linea delle lunghezza cercata*/
            }
            j = 0; /*azzeriamo l'indice per la prossima linea*/
            i++; /*e incrementiamo il conteggio delle linee*/
        }
        else j++; /*altrimenti incrementiamo l'indice nel buffer*/
    }

    if(!trovata){
        printf("Non è stata trovata alcuna linea nel file %s di linghezza %d\n", argv[1], n);
    }
    /*---------------------------------------------------------------------------------------------------------------*/


               /*ogni figlio apre lo stesso file*/
                if((fd = open(argv[1], O_RDONLY)) < 0)
                {
                    printf("Errore nella apertura del file %s\n", argv[1]);
                    exit(-1);
                }

                /*LOGICA DI IMPLEMENTAZIONE SE VOGLIAMO CREARE UN FILE*/
                /*creaimo un file Fcreato i cui nome sia la concatenazione del nome del file associato Ff con la stringa ".MINORE"*/
                
                /*bisogna allocare una stringa lunga come il nome del fule + il carattere '.' + i caratteri della parola MINORE(6) + IL TERMINTAORE DELLA STRINGA*/
                char *Fcreato = (char *)malloc(strlen(argv[i+1]) + 10);
                if(Fcreato == NULL)
                {
                    printf("Errore nella malloc\n");
                    exit(0);
                }
                /*copiamo il nome del file associato al figlio*/
                strcpy(Fcreato,argv[i+1]);
                strcat(Fcreato,".MINORE");
                int fdw = creat(Fcreato,0644);
                if(fdw < 0)
                {
                    printf("Impossibile creare il file %s\n", Fcreato);
                }


                /*NEL CASO DI UNA STRUCT CIOè SINGOLA PIPE*/

                 /*inizializza la struttura*/
                msg.c = argv[n+2][0]; /*carattere associato*/
                msg.n = 0L; /*contatore delle occorenze */
                /*conta le occorenze del carattere associato*/
                while(read(fd,&c,1) > 0)/*si legge il file un carattere alla volta fino alla fine*/
                {
                    if (c == argv[n+2][0]) /*se si è trovato il carattere associato*/
                    msg.n++; /*si incrementa il contatore*/
                }

                /*comunica al padre*/
                write(p[1], &msg, sizeof(msg));
                exit(argv[n+2][0]); /*torniamo il carattere cercato*/

                /*NEL CASO DI UNA ARRAY DI N PIPE*/

                cont = 0L; /*conta le occorenze del carattere associato e quindi va inizializzato a 0*/
                while(read(fd,&c,1) > 0) /*si legge il file un carattere alla volta fino alla fine*/
                {
                    if (c == argv[n+2][0]) /*se si è trovato il carattere associato*/
                        cont++;
                }

                /*comunica al padre usando la n-esima pipe*/
                 write(piped[n][1], &cont, sizeof(cont));
                 exit(argv[n+2][0]);

            }
        }

        /*dopo aver concluso il ciclo for di creazione si tutti i figli, si considera il resto del codice che ddeve eseguire il padre!*/
    /*padre*/
    /*chiude il lato della pipe che non usa*/

    /*NEL CASO DI UNA SINGOLA PIPE*/
        close(p[1]);

    /*NEL CASO DI UNA ARRAY DI N PIPE*/
     for (i=0; i < N ; ++i)
    {
        close(piped[i][1]);
    }

    /*NEL CASO DI UNO SCHEMA DI COMMUNICAZIONE A RING*/
    for (i=0; i < i+1; i++) 
	{
		if (i != N)	close (piped[i][0]);
		if (i != 0) 	close (piped[i][1]); 
	}

    /*NEL CASO DEL PIPELINE*/
    /*chiusura lati delle pipe non usati: tutti meno quello l'ultimo in lettura*/
    for(i=0; i<N; i++)
    {
        close(piped[i][1]);
        if(i != N-1) close (piped[i][0]);
    }

    /*NEL CASO DOVE ABBIAMO 2*N PROCESSI E VOGLIAMO LAVORARE SU DUE PARTE DISTINTI CON 2 ARRAY DI PIPE ALLOCATI PER LO SCHEMA DI COMMUNICAZIONE*/
               /*chiudiamo le pipe che non servono*/
    /*chiudiamo tutte le pipe, dato che le usano solo i figli*/

    for(i=0; i<N; i++)
     {
        close(pipe_f[i][0]);
        close(pipe_f[i][1]);
        close(pipe_fbis[i][0]);
        close(pipe_fbis[i][1]);
     }


    /*SCRIVERE IL CODICE DELLA SPECIFICA DEL PADRE*/

    /*PEZZO DI CODICE PER VEDERE COME IL PADRE RICEVE GLI INFORMAZIONI NEL CASO DI UNA COMMUNICAZIONE A PIPELINE BASANDOCI SUL NOSTRO ESEMPIO*/
     /*il padre deve leggere l'array di struttura che gli arriva dall'ultimo figlio*/
    /*quindi al padre arriva una singola informazione, rappresentata però da un array che quindi contiene più elementi*/
    nr=read(piped[N-1][0],d,sizeof(d));
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
            //printf("Il figlio di indice %d e pid %d ha trovato %ld occorenze del carattere %c\n", d[i].ch-'a', pid[(d[i].ch-'a')], d[i].occ, d[i].ch);
            /*Ho commentato questo a causa della variabile che deve essere un array di pid in questo caso*/
        }   
    }

    /*---------------------------------------------------------*/
    /*PEZZO DI CODICE PER LA LETTURA DALLA PIPE I MESSAGGI*/
    /*NEL CASO DELLA SINGOLA PIPE*/
    while (read(p[0], &msg, sizeof(msg)) > 0)
        printf("Trovato %ld occorenze del carattere %c nel file %s\n", msg.n, msg.c, argv[1]);

    /*NEL CASO DI UN ARRAY DI N PIPE*/
    for(i=0; i < N; i++)
    {
        if(read(piped[i][0], &cont, sizeof(cont)) > 0)
            printf("Trovate %ld occorenze del carattere %c nel file %s\n", cont, argv[n+2][0], argv[1]);
    }
    /*----------------------------------------------------------*/

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
                    printf("Figlio con pid %d ha ritornato %d\n", pid , ritorno);
   		}
  	} 

    exit(0);

    

}

