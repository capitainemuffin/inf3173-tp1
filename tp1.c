#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *runner(void *param); /* the thread */

int main(int argc, char *argv[])
{


	if(argc != 2){
		fprintf(stderr, "Un seul argument est permis : le fichier contenant le sudoku à analyser.\n");
		exit(-1);
	}

	FILE* fichier = fopen(argv[1], "r");

	if(fichier == NULL){
		fprintf(stderr, "Une erreur est survenue à l'ouverture du fichier.\n");
		exit(-2);

	}

	char chiffre;
	int colonne = 1;
	int ligne = 1;
	char sudoku[8][9];


	while((chiffre = fgetc(fichier)) != EOF){

		if(chiffre == ' ' || chiffre == '\v' || chiffre == '\t'){
			fprintf(stderr, "Les espaces entre les chiffres ne sont pas permis.\n");
			exit(-3);
		} 

		if(chiffre == '\n' || chiffre == '\r'){

			if(colonne != 10){
				fprintf(stderr, "La taille de la ligne %d n'est pas conforme : %d\n", ligne, colonne);
				exit(-4);
			}

			ligne += 1;
			colonne = 1;

		} else {

			if(chiffre < '1' || chiffre > '9') {
				fprintf(stderr, "La valeur à la ligne %d colonne %d n'est pas un chiffre : %c\n", ligne, colonne, chiffre);
				exit(-5);

			}

			if(colonne > 9){
				fprintf(stderr, "La taille de la ligne %d est trop longue.\n", ligne);
				exit(-6);

			}

			if(ligne > 9){
				fprintf(stderr, "Le maximum de 9 lignes a été dépassé.\n");
				exit(-7);			
			}			


			colonne += 1;

		}

	}

	if(ligne != 10){
		fprintf(stderr, "La nombre de lignes n'est pas conforme : %d\n", ligne);
		exit(-8);
	}

	fclose(fichier);
	return 0;	
}