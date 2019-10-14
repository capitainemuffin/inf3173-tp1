#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 9
int sudoku[MAX][MAX];
int sections_valides[MAX * 3];

/**
* Thread qui valide une ligne
**/
void *verifierLigne(void *i);

/**
* Thread qui valide une colonne
**/
void *verifierColonne(void *i);

/**
* Thread qui valide une sous matrice 
**/
void *verifierSousMatrice(void *i);

/**
* Mets le sudoku d'un fichier le tableau 'sudoku'.
* Valide la taille et le format en même temps.
* Renvoi 0 si le format est valide, une valeur négative sinon.
**/
int getSudoku(FILE *fichier);

/**
* Vérifie le tableau 'section_valides' et renvoi 0 si toutes les valeurs sont 0. 
* Renvoi -1 sinon.
**/
int sudokuValide();

/**
* Déplace le pointeur du fichier au prochain Sudoku
**/
void prochainSudoku(FILE *fichier);

/**
* Éxecution
**/
int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Un seul argument est permis : le fichier contenant le sudoku à analyser.\n");
        exit(1);
    }

    FILE *fichier = fopen(argv[1], "rb");

    if (fichier == NULL) {
        fprintf(stderr, "Une erreur est survenue à l'ouverture du fichier.\n");
        exit(2);

    }

    int caractere, format_valide, index_sudoku = 1;

    while ((caractere = fgetc(fichier)) != EOF) {

        if (isspace(caractere)) continue;

        fprintf(stdout, "Sudoku %d : \n", index_sudoku);
        fseek(fichier, -1, SEEK_CUR);
        format_valide = getSudoku(fichier);

        if (format_valide == 0) {

            pthread_t threads[MAX * 3];

            for (int index = 0; index < MAX; index++) {

                pthread_create(&threads[index], NULL, &verifierLigne, &index);
                pthread_create(&threads[index + MAX], NULL, &verifierColonne, &index);
                //pthread_create(&threads[index + 2 * MAX], NULL, &verifierSousMatrice, &index);

            }

            for (int i = 0; i < MAX * 3; i++) pthread_join(threads[i], NULL);

            if (sudokuValide() == 0) fprintf(stdout, "Bravo! Votre Sudoku est valide!\n");

        }

        prochainSudoku(fichier);
        index_sudoku++;

    }

    fclose(fichier);
    return 0;
}

int getSudoku(FILE *fichier) {

    int chiffre, colonne = 1, ligne = 1;

    while ((chiffre = fgetc(fichier)) != EOF) {

        //Ignorer les espaces
        if (chiffre == ' ' || chiffre == '\v' || chiffre == '\t') continue;

        if (chiffre == '\n' || chiffre == '\r') {

            if (chiffre == '\n' && fgetc(fichier) != '\r') fseek(fichier, -1, SEEK_CUR);
            if (chiffre == '\r' && fgetc(fichier) != '\n') fseek(fichier, -1, SEEK_CUR);

            if (colonne != MAX + 1) {
                fprintf(stderr, "La taille de la grille de Sudoku devrait être %dx%d\n", MAX, MAX);
                return -1;
            }

            //Détecter si dernière ligne du Sudoku (2 sauts de lignes de suite)
            chiffre = fgetc(fichier);
            if ((chiffre == '\n' || chiffre == '\r') && ligne != MAX) {
                fprintf(stderr, "La taille de la grille de Sudoku devrait être %dx%d\n", MAX, MAX);
                fseek(fichier, -1, SEEK_CUR);
                return -2;
            }
            fseek(fichier, -1, SEEK_CUR);

            //le sudoku est complet
            if (ligne == MAX) {
                fseek(fichier, -1, SEEK_CUR);
                return 0;
            }

            ligne++;
            colonne = 1;

        } else {

            if (!isdigit(chiffre)) {
                fprintf(stderr, "La case (%d,%d) contient un caractère non-entier (%c)\n", ligne, colonne, chiffre);
                return -3;

            }

            if (colonne > MAX || ligne > MAX) {
                fprintf(stderr, "La taille de la grille de Sudoku devrait être %dx%d\n", MAX, MAX);
                return -4;
            }

            sudoku[ligne - 1][colonne - 1] = chiffre;
            colonne++;

        }

    }

    if (ligne != MAX || colonne != MAX + 1) {

        fprintf(stderr, "La taille de la grille de Sudoku devrait être %dx%d\n", MAX, MAX);
        return -5;
    }

    return 0;

}

void *verifierLigne(void *i) {

    int ligne = *(int *) i;

    for (int z = 0; z < MAX; z++) {
        for (int j = z + 1; j < MAX; j++) {

            if (sudoku[ligne][z] == sudoku[ligne][j]) {
                printf("Ligne analysée : %d\n", ligne);
                fprintf(stderr, "Il y a un doublon à la ligne %d. Le chiffre %c est présent plus d'une fois.\n",
                        ligne + 1, sudoku[ligne][z]);
                sections_valides[ligne] = -1;
                pthread_exit(NULL);
            }

        }
    }

    sections_valides[ligne] = 0;
    pthread_exit(NULL);
}

void *verifierColonne(void *i) {

    int colonne = *(int *) i;

    for (int z = 0; z < MAX; z++) {
        for (int j = z + 1; j < MAX; j++) {

            if (sudoku[z][colonne] == sudoku[j][colonne]) {

                fprintf(stderr, "Il y a un doublon à la colonne %d. Le chiffre %c est présent plus d'une fois.\n",
                        colonne + 1, sudoku[z][colonne]);
                sections_valides[colonne + MAX] = -1;
                pthread_exit(NULL);

            }

        }
    }

    sections_valides[colonne + MAX] = 0;
    pthread_exit(NULL);

}

int sudokuValide() {

    for (int i = 0; i < MAX * 3; i++) {

        if (sections_valides[i] != 0) return -1;
    }

    return 0;
}

void prochainSudoku(FILE *fichier) {

    int caractere;
    //Passer au prochain sudoku, détecter 2 sauts de ligne de suite
    fseek(fichier, -1, SEEK_CUR);
    while ((caractere = fgetc(fichier)) != EOF) {
        if (caractere == '\n' || caractere == '\r') {
            if (caractere == '\n' && fgetc(fichier) != '\r') fseek(fichier, -1, SEEK_CUR);
            if (caractere == '\r' && fgetc(fichier) != '\n') fseek(fichier, -1, SEEK_CUR);
            caractere = fgetc(fichier);
            if (caractere == '\n' || caractere == '\r') {
                if (caractere == '\n' && fgetc(fichier) != '\r') fseek(fichier, -1, SEEK_CUR);
                if (caractere == '\r' && fgetc(fichier) != '\n') fseek(fichier, -1, SEEK_CUR);
                break;
            }
        }
    }

}
