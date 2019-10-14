#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 9
char sudoku [MAX][MAX];
char sections_valides[MAX * 3];
/**
* Thread qui valide une ligne
**/
void* verifierLigne(void* i);

/**
* Thread qui valide une colonne
**/
void* verifierColonne(void* i);

/**
* Thread qui valide une sous matrice 
**/
void* verifierSousMatrice(void* i);

/**
* Mets le sudoku d'un fichier le tableau 'sudoku'.
* Valide la taille et le format en même temps.
* Renvoi 0 si le format est valide, une valeur négative sinon.
**/
int getSudoku(FILE* fichier);

/**
* Vérifie le tableau 'section_valides' et renvoi 0 si toutes les valeurs sont 0. 
* Renvoi -1 sinon.
**/
int sudokuValide();

/**
* Déplace le pointeur du fichier au prochain Sudoku
**/
void prochainSudoku(FILE* fichier);

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

	char caractere; 
	int sudoku_format_valide;

	while((caractere = fgetc(fichier)) != EOF){

		if(isspace(caractere)) continue;

		#ifdef DEBUG
			printf("==== DEBUT SUDOKU === \n");
		#endif

		fseek(fichier, -1, SEEK_CUR);
		sudoku_format_valide = getSudoku(fichier);

		#ifdef DEBUG
			printf("Sudoku valeur de retour : %d\n", sudoku_format_valide);
		#endif

		if(sudoku_format_valide == 0){
			//lancer les threads
			#ifdef DEBUG
			printf("==== VALIDE === \n");
			#endif

			pthread_t threads [MAX * 3];
			
			for (int i = 0 ; i < 9 ; i++){

				pthread_create(&threads[i * 1], NULL, &verifierLigne, &i);
				pthread_create(&threads[i + MAX], NULL, &verifierColonne, &i);
				//pthread_create(&threads[i + 2 * MAX], NULL, &verifierSousMatrice, &i);

			}

			for (int i = 0 ; i < MAX * 3 ; i++){

				pthread_join(threads[i], NULL);

			}

			if(sudokuValide() == 0) fprintf(stdout, "Bravo! Votre Sudoku est valide!\n");

		}

		#ifdef DEBUG
			printf("==== FIN SUDOKU === \n");
		#endif

		prochainSudoku(fichier);

	}

	fclose(fichier);
	return 0;	
}

int getSudoku(FILE* fichier){

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

			//le sudoku est complet
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

			sudoku[ligne - 1][colonne - 1] = chiffre;
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

void* verifierLigne(void* i){

	int ligne = *(int*)i;

	for(int i = 0 ; i < MAX ; i++){
		for (int j = i + 1; j < MAX; j++){

			if(sudoku[ligne][i] == sudoku[ligne][j]){

				fprintf(stderr, "Il y a un doublon à la ligne %d. %c et %c.\n", ligne + 1, sudoku[ligne][i], sudoku[ligne][j]);
				sections_valides[ligne] = -1;
				pthread_exit(NULL);
			}

		}
	}

	sections_valides[ligne] = 0;
	pthread_exit(NULL);
}

void* verifierColonne(void* i){

	int colonne = *(int*)i;

	for(int i = 0 ; i < MAX ; i++){
		for (int j = i + 1; j < MAX; j++){

			if(sudoku[i][colonne] == sudoku[j][colonne]){

				fprintf(stderr, "Il y a un doublon à la colonne %d. %c et %c.\n", colonne + 1, sudoku[i][colonne], sudoku[j][colonne]);
				sections_valides[colonne + MAX] = -1;
				pthread_exit(NULL);

			}

		}
	}

	sections_valides[colonne + MAX] = 0;
	pthread_exit(NULL);

}

int sudokuValide(){

	for (int i = 0 ; i < MAX * 3 ; i++){

		if(sections_valides[i] != 0) return -1;
	}

	return 0;
}

void prochainSudoku(FILE* fichier){

	char caractere;
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

}
