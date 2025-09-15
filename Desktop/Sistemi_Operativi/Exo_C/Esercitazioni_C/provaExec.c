#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char *argin[2]; /*array statico di puntatori a char che serve per passare i parametri della execv*/
    int x; /*per leggere un valore dallo stdin */

    argin[0] = "provaExec";
    argin[1]=(char *)0; /*il terminatore della lista di stringhe*/

    printf("Esecuzione di %s\n", argin[0]);
    printf("Dimmi se vuoi finire (valore 0)!\n");
    scanf("%d", &x);
    if (x!= 0){
        printf("DEBUG-Sto per eseguire %s\n", argin[0]);
        execvp(argin[0], argin);

        /*si esegue l'istruzione seguente SOLO in caso di fallimento della execvp*/
        printf("Errore in execvp\n");
        exit(1);
    }

    exit(x);
}