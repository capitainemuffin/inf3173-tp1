# Travail Pratique 1

## Description

Implémentation en C d'un validateur de Sudoku

`Cours` : Principes des systèmes d'exploitation

`Sigle` : INF3173

`Session` : Automne 2019

`Université` : UQAM

`Enseignant` : Ammar Hamad

## Auteur

Sofiane Selaoui (SELS28049204)

### Fonctionnement 

Le programme valide un ou plusieurs Sudokus contenus dans un fichier texte.

Pour compiler le programme : 

```
gcc tp1.c -lpthread -o tp1
```
ou, si le fichier `Makefile` est présent :  

```
make
```

Pour lancer le programme :

```
./tp1 fichier
```

Pour que le programme fonctionne : 

- Le fichier doit contenir un/des sudoku(s) de taille 9x9.
- Les sudokus doivent êtres séparés par une ligne vide.
- Les sudokus doivent comporter des chiffres seulement.

Pour qu'un sudoku soit valide : 

- Une ligne ne peut pas contenir 2 fois le même chiffre.
- Une colonne ne peut pas contenir 2 fois le même chiffre.
- Les 9 sous-matrices 3x3 ne peuvent pas contenir 2 fois le même chiffre.

### Exemples

Commande : 

`./tp1 sudokus.txt`

Sortie : 

```
Sudoku 1 : 
Bravo! Votre Sudoku est valide !
Sudoku 2 : 
Il y a un doublon à la colonne 4. Le chiffre 1 est présent plus d'une fois.
Sudoku 3 :
Il y a un doublon à la sous-matrice 9. Le chiffre 7 est présent plus d'une fois.
Sudoku 4 : 
La case (4,4) contient un caractère non-entier (&)
Sudoku 5 : 
La taille de la grille de Sudoku devrait être 9x9
```

### Références
- [Cplusplus](http://www.cplusplus.com/reference/clibrary)
- [Tutorialspoint](https://www.tutorialspoint.com/cprogramming)
- [LeSudoku](http://www.le-sudoku.fr/le-sudoku/regles)



