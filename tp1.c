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

	FILE* fichier = fopen(argv[1], "rb");

	if(fichier == NULL){
		fprintf(stderr, "Une erreur est survenue à l'ouverture du fichier.\n");
		exit(2);

	}

	char caractere, sudoku [MAX][MAX]; 
	int sudoku_format_valide;

	while((caractere = fgetc(fichier)) != EOF){

		if(isspace(caractere)) continue;

		#ifdef DEBUG
			printf("==== DEBUT SUDOKU === \n");
		#endif

		fseek(fichier, -1, SEEK_CUR);
		sudoku_format_valide = getSudoku(sudoku, fichier);

		#ifdef DEBUG
			printf("Sudoku valeur de retour : %d\n", sudoku_format_valide);
		#endif

		if(sudoku_format_valide == 0){
			//lancer les threads
			#ifdef DEBUG
			printf("==== VALIDE === \n");
			#endif

			int sudoku_valide [MAX * 3];
			// for(int i = 0 ; i < 9 ; i++) {

			// 	verifierColonne(i, sudoku_valide);
			// 	verifierLigne(i, sudoku_valide);
			// 	verifierSousMatrice(i, sudoku_valide);

			// }

		}

		//Passer au prochain sudoku, détecter 2 sauts de ligne de suite
		fseek(fichier, -1, SEEK_CUR);
		while((caractere = fgetc(fichier)) != EOF){
			if(caractere == '\n' || caractere == '\r'){
				if(caractere == '\n' && fgetc(fichier) != '\r') fseek(fichier,-1,SEEK_CUR);
				if(caractere == '\r' && fgetc(fichier) != '\n') fseek(fichier,-1,SEEK_CUR);
				caractere = fgetc(fichier);
				if(caractere == '\n' || caractere == '\r'){
					if(caractere == '\n' && fgetc(fichier) != '\r') fseek(fichier,-1,SEEK_CUR);
					if(caractere == '\r' && fgetc(fichier) != '\n') fseek(fichier,-1,SEEK_CUR);
					break;
				}
			}
		}

		#ifdef DEBUG
			printf("==== FIN SUDOKU === \n");
		#endif

	}

	fclose(fichier);
	return 0;	
}

int getSudoku(char matrice[MAX][MAX], FILE* fichier){

	char chiffre;
	int colonne = 1, ligne = 1;

	while((chiffre = fgetc(fichier)) != EOF){

		#ifdef DEBUG
		if(chiffre == '\n') printf("\\n ");
		if(chiffre == '\r') printf("\\r ");
		#endif

		//Ignorer les espaces
		if(chiffre == ' ' || chiffre == '\v' || chiffre == '\t') continue;

		if(chiffre == '\n' || chiffre == '\r'){

			if(chiffre == '\n' && fgetc(fichier) != '\r') fseek(fichier,-1,SEEK_CUR);
			if(chiffre == '\r' && fgetc(fichier) != '\n') fseek(fichier,-1,SEEK_CUR);

			#ifdef DEBUG
			printf("finligne, ligne : %d, colonne : %d\n", ligne, colonne);
			#endif

			if(colonne != MAX + 1){
				fprintf(stderr, "La taille de la grille de Sudoku devrait être 9x9\n");
				return -1;
			}

			//Détecter si dernière ligne du Sudoku (2 sauts de lignes de suite)
			chiffre = fgetc(fichier);
			if((chiffre == '\n' || chiffre == '\r') && ligne != MAX){
				#ifdef DEBUG
				if(chiffre == '\n') printf("\\n");
				if(chiffre == '\r') printf("\\r");
				#endif
				fprintf(stderr, "La taille de la grille de Sudoku devrait être 9x9\n");
				fseek(fichier,-1,SEEK_CUR);
				return -2;
			}
			fseek(fichier,-1,SEEK_CUR);

			//la matrice est complète
			if(ligne == MAX){
				fseek(fichier,-1,SEEK_CUR);
				return 0;
			}

			ligne ++;
			colonne = 1;

		} else {

			#ifdef DEBUG
			printf("[%c] (%d,%d) ", chiffre,ligne,colonne);
			#endif

			if(!isdigit(chiffre)) {
				fprintf(stderr, "La case (%d,%d) contient un caractère non-entier (%c)\n", ligne, colonne, chiffre);
				return -3;

			}

			if(colonne > MAX || ligne > MAX){
				fprintf(stderr, "La taille de la grille de Sudoku devrait être 9x9\n");
				return -4;
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
		return -5;
	}

	return 0;

}
