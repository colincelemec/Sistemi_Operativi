#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h> /*per la inizializzazione del seme per la rand*/

int mia_random(int n)
{
int casuale;
casuale = rand() % n;
casuale++;
return casuale;
}



int main(int argc, char **argv){
    int N; /*numerp di figli*/
    int *pid; /* array dinamico di pid per fork */
    int n; /* indice per i figli */
    int j; /*indice per ricuperare il numero d'ordine di creazione dei figli*/
    int pidFiglio, status, ritorno; /*per wait e per valori di ritorno*/
    /*controllo sul numero di parametri*/

    if(argc != 2){
        printf("Errore sul numero di parametri perché ci vuole almeno 1!");
        exit(1);
    }
    /*controllo sul tipo di parametri*/
     N = atoi(argv[1]);
    if(N <= 0 || N >= 155){
        printf("Il primo parametro %s non è un numero strettamente positivo oppure è maggiore o uguale a 155\n", argv[1]);
        exit(2);
    }

    /*allocazione pid*/
    pid = (int *)malloc(N * sizeof(int));
    if(pid == NULL){
        printf("Errore allocazione pid\n");
        exit(3);
    }

    /*riportare su stdout il pid del processo corrente insieme con il numero N*/
    printf("sono il processo padre con pid %d e sto per generare %d figli\n", getpid() , N);
    srand(time(NULL));

    /*il processo padre deve generare N processo figli*/
    /* creazione figli */
    for(n=0; n < N; ++n){
        if((pid[n]=fork()) < 0){
            printf("ERRORE NELLA FORK\n");
            exit(4);
        }

        if(pid[n]== 0){
            /*codice figlio*/
            int r;
            printf("sono il figlio %d di indice %d\n", getpid(), n);
            r = mia_random(100+n);
            /*ogni figlio deve ritornare il numero random calcolato*/
            exit(r);
            
        }
    }

    /*codice padre*/
    	printf("DEBUG-Sono il processo padre con pid %d e, dato che ho finito di generare i miei %d figli, ora li posso aspettare uno per uno in ordine!\n", getpid(), N);

    /*il padre aspetta gli N figli*/
    for(n=0; n < N ; ++n){
        pidFiglio=wait(&status);
        if (pidFiglio < 0){
            printf("ERRORE NELLA WAIT\n");
            exit(5);
        }
        if ((status & 0xFF) != 0)
        printf("Figlio  con pid %d terminato in modo anomalo\n",pidFiglio);
        else{
            ritorno=(int)((status >> 8) & 0xFF);
            /*la specifica richiede che il padre , oltre al valore di ritorno e il PID , stampi anche il numero d'ordine e questo può essere fatto solo analizzando il contenuto dell'array di pid e riportando l'indice di tale array. ATTENZIONE CHE SAREBBE ASSOLUTAMENTE ERRATO ANDARE A STAMPARE L'INDICE DEL FOR ESTERNO! */
			for (j=0; j < N; j++)
				if (pidFiglio == pid[j]){
                    printf("il figlio %d di indice %d ha ritotnato %d\n", pidFiglio, j, ritorno);
                    break;
                }
            
        }
    }

    exit(0);
}
