# Travail Pratique 1

## Description

Implémentation en C d'un validateur de Sudoku

`Cours` : Principes des systèmes d'exploitation

`Sigle` : INF3173

`Session` : Automne 2019

`Université` : UQAM

## Auteur

Sofiane Selaoui (SELS28049204)

### Fonctionnement 

Le programme valide un ou plusieurs Sudokus contenus dans un fichier texte.

Pour compiler le programme : 

`gcc tp1.c -lpthread -o tp1`

Pour lancer le programme :

`./tp1 fichier`

Pour que le programme fonctionne : 

- Le fichier doit contenir un/des sudoku(s) de taille 9x9.
- Les sudokus doivent êtres séparés par une ligne vide.
- Les sudokus doivent comporter des chiffres seulement.

### Exemples

Commande : 

`./tp1 sudokus.txt`

Sortie : 

```
Sudoku 1 : Valide !
Sudoku 2 : Invalide à la ligne 2
Sudoku 3 : Invalide à la colonne 4
Sudoku 3 : Invalide à la sous-matrice 6
```

### Références
- [Cplusplus](http://www.cplusplus.com/reference/clibrary)
- [Tutorialspoint](https://www.tutorialspoint.com/cprogramming)
- [LeSudoku](http://www.le-sudoku.fr/le-sudoku/regles)



