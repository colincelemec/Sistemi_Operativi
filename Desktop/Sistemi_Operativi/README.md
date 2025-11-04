# Sistemi Operativi

Repository contenente materiale ed esercizi per il corso di Sistemi Operativi.

## Descrizione

Questo repository contiene una collezione di esercizi, prove d'esame e materiale didattico per il corso di Sistemi Operativi. Il lavoro include programmazione in C e scripting Shell, con particolare focus su:

- Gestione dei processi (fork, exec, wait)
- Comunicazione tra processi (pipe, named pipe)
- Sincronizzazione
- Gestione di file e directory
- Scripting Shell avanzato

## Struttura del Repository

```
Sistemi_Operativi/
├── Exo_C/                    # Esercizi in linguaggio C
├── Exo_shell/                # Esercizi in Shell scripting
├── File_base_C/              # File di base per esercizi in C
├── File_base_shell/          # File di base per esercizi in Shell
├── LEZIONI/                  # Materiale delle lezioni
├── Quiz_sistemi/             # Quiz e test di autovalutazione
├── PROVE_TOTALE/             # Prove complete ed esami simulati
├── Correzione_esame_12Giu24/ # Correzione esame del 12 Giugno 2024
├── prepa_Exam/               # Materiale di preparazione agli esami
├── studente_01_38_304874/    # Progetti ed esami studente
├── DIR_DI_PROVA_SHELL/       # Directory di test per Shell
└── *.pdf                     # Testi degli esami passati
```

## Argomenti Trattati

### Programmazione in C
- Creazione e gestione di processi con `fork()`
- Comunicazione tra processi tramite pipe
- Manipolazione di file (open, read, write, close)
- Gestione dei segnali
- Allocazione dinamica della memoria
- System calls Unix/Linux

### Shell Scripting
- Script Bash avanzati
- Elaborazione di file e directory
- Uso di comandi Unix (grep, sed, awk, etc.)
- Controllo di flusso e cicli
- Gestione di parametri e variabili

## Esempi di Progetti

### studente_01_38_304874
Progetto che implementa:
- Comunicazione tra processi padre e figli tramite pipe
- Elaborazione di file con caratteri in posizione pari/dispari
- Sincronizzazione tra processi
- Creazione di file di output

**File principali:**
- `main.c` - Programma principale con gestione di processi multipli
- `FCP.sh` e `FCR.sh` - Script Shell per elaborazione file
- `makefile` - Compilazione automatica del progetto

## Requisiti

- **Sistema Operativo**: Unix/Linux o macOS
- **Compilatore C**: gcc
- **Shell**: Bash

## Compilazione ed Esecuzione

### Progetti in C

```bash
cd studente_01_38_304874
make
./main file1.txt file2.txt
```

### Script Shell

```bash
chmod +x script.sh
./script.sh parametro1 parametro2
```

## Esami

Il repository include testi e correzioni di esami passati:
- 11 Giugno 2025 (`11Giu25-web.pdf`)
- 15 Aprile 2016 (`15Apr2016-web.pdf`)
- 12 Giugno 2024 (con correzione in `Correzione_esame_12Giu24/`)

## Note

Questo repository è stato creato a scopo didattico per lo studio e la pratica dei concetti di Sistemi Operativi.

## Autore

Colince Tcheussieu Mendji

## Licenza

Materiale didattico per uso educativo.
