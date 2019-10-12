#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 9

/**
* Thread qui valide une ligne
**/
void* verifierLigne(void* param);

/**
* Thread qui valide une colonne
**/
void* verifierColonne(void* param);

/**
* Thread qui valide une sous matrice 
**/
void* verifierSousMatrice(void* param);

/**
* Mets le sudoku d'un fichier dans une matrice.
* Valide la taille et le format en même temps.
* Renvoi 1 si le format est valide, une valeur négative sinon.
**/
int getSudoku(char matrice[MAX][MAX], FILE* fichier);

/**
* Éxecution
**/
int main(int argc, char *argv[])
{

	if(argc != 2){
		fprintf(stderr, "Un seul argument est permis : le fichier contenant le sudoku à analyser.\n");
		exit(1);
	}

	FILE* fichier = fopen(argv[1], "r");

	if(fichier == NULL){
		fprintf(stderr, "Une erreur est survenue à l'ouverture du fichier.\n");
		exit(2);

	}

	char sudoku [MAX][MAX]; 
	char caractere;
	int sudoku_format_valide;
	while((caractere = fgetc(fichier)) != EOF){

		if(!isspace(caractere)){

			#ifdef DEBUG
				printf("==== DEBUT SUDOKU === \n");
			#endif

			//Remettre le pointeur en bonne position pour la lecture
			fseek(fichier, -1, SEEK_CUR);
			sudoku_format_valide = getSudoku(sudoku, fichier);

			#ifdef DEBUG
				printf("Sudoku format valide : %d", sudoku_format_valide);
			#endif

			if(sudoku_format_valide != 1){

				//Passer au prochain Sudoku
				while(fgetc(fichier) != '\n' || fgetc(fichier) != '\n'){
					fseek(fichier, -1, SEEK_CUR);
				}
			}

			#ifdef DEBUG
				printf("==== FIN SUDOKU === \n");
			#endif
		}

	}

	fclose(fichier);

	return 0;	
}

int getSudoku(char matrice[MAX][MAX], FILE* fichier){

	char chiffre;
	int colonne = 1, ligne = 1;

	while((chiffre = fgetc(fichier)) != EOF){

		//Ignorer les espaces dans le fichier
		if(chiffre == ' ' || chiffre == '\v' || chiffre == '\t') continue;

		if(chiffre == '\n' || chiffre == '\r'){

			#ifdef DEBUG
			printf("finligne, ligne : %d, colonne : %d\n", ligne, colonne);
			#endif

			if(colonne != MAX + 1){
				fprintf(stderr, "La taille de la grille de Sudoku devrait être 9x9\n");
				return -1;
			}

			//la matrice est complète
			if(ligne == MAX){
				//Remettre le pointeur en bonne position 
				fseek(fichier,-1,SEEK_CUR);
				return 1;
			} 
			
			ligne ++;
			colonne = 1;

		} else {

			#ifdef DEBUG
			printf("[%c] (%d,%d) ", chiffre,ligne,colonne);
			#endif

			if(!isdigit(chiffre)) {
				fprintf(stderr, "La case (%d,%d) contient un caractère non-entier (%c)\n", ligne, colonne, chiffre);
				return -2;

			}

			if(colonne > MAX || ligne > MAX){
				fprintf(stderr, "La taille de la grille de Sudoku devrait être 9x9\n");
				return -1;

			}		

			matrice[ligne - 1][colonne - 1] = chiffre;
			colonne ++;

		}

	}

	if(ligne != MAX || colonne != MAX + 1){
		#ifdef DEBUG
		printf("fin fichier, ligne : %d, colonne : %d\n", ligne, colonne);
		#endif
		fprintf(stderr, "La taille de la grille de Sudoku devrait être 9x9\n");
		return -1;
	}

	return 1;

}
