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
**/
void getSudoku(char matrice[MAX][MAX], FILE* fichier);

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

	while((caractere = fgetc(fichier)) != EOF){

		//Ignorer les sauts de lignes et espaces dans le fichiers avant le sudoku
		if(!isspace(caractere)){

			//Remettre le pointeur en bonne position pour la lecture
			fseek(fichier, -1, SEEK_CUR);
			getSudoku(sudoku, fichier);

		}

	}

	fclose(fichier);

	return 0;	
}

void getSudoku(char matrice[MAX][MAX], FILE* fichier){

	char chiffre;
	int colonne = 0;
	int ligne = 0;

	while((chiffre = fgetc(fichier)) != EOF){

		//Ignorer les espaces dans le fichier
		if(chiffre == ' ' || chiffre == '\v' || chiffre == '\t') continue;

		if(chiffre == '\n' || chiffre == '\r'){

			if(colonne != MAX){
				fprintf(stderr, "La longueur de la ligne %d n'est pas conforme. Longueur : %d\n", ligne, colonne);
				exit(4);
			}

			//la matrice est complète
			if(ligne == MAX - 1){
				//Remettre le pointeur en bonne position 
				fseek(fichier,-1,SEEK_CUR);
				return;
			} 
			
			ligne ++;
			colonne = 0;

		} else {

			if(!isdigit(chiffre)) {
				fprintf(stderr, "La valeur à la ligne %d colonne %d n'est pas un chiffre : %c\n", ligne, colonne, chiffre);
				exit(5);

			}

			if(colonne > MAX){
				fprintf(stderr, "La longueur de la ligne %d n'est pas conforme. Longueur : %d\n", ligne, colonne);
				exit(4);

			}

			if(ligne > MAX){
				fprintf(stderr, "Le maximum de 9 lignes a été dépassé.\n");
				exit(7);			
			}			

			matrice[ligne][colonne] = chiffre;
			colonne ++;

		}

	}

	//EOF atteint 
	if(ligne != MAX - 1){
		fprintf(stderr, "La nombre de lignes n'est pas conforme. Taille : %d\n", ligne);
		exit(8);
	}

	if(colonne != MAX){
		fprintf(stderr, "La longueur de la ligne %d n'est pas conforme. Longueur : %d\n", ligne, colonne);
		exit(4);	
	}

}
