## Compilation et installation des dépendances

Avant de lancer le projet, placez-vous dans le dossier `src` :

```bash
cd src
```

Compilez ensuite le projet :

```bash
make
```

Installez les dépendances Node.js :

```bash
npm i
```

## Configuration de l'algorithme

Toutes les constantes de configuration de l'algorithme se trouvent dans le fichier :

```text
constants.h
```

### Nombre d'individus et de générations

Pour modifier le nombre d'individus par génération, changez la valeur de :

```c
INDIVIDUALS_SIZE
```

Pour modifier le nombre total de générations, changez la valeur de :

```c
NB_GENERATIONS
```

### Important

Si vous modifiez `INDIVIDUALS_SIZE`, vous devez également adapter les constantes suivantes afin que leur somme soit cohérente avec le nombre total d'individus par génération :

```c
#define NB_ELITE 5      /* Number of best individuals of the previous generation kept in the next generation */
#define NB_CROSSOVER 15 /* Number of crossovers between two individuals of the previous generation added to the next generation */
#define NB_MUTATION 20  /* Number of mutations of the best individuals of the previous generation added to the next generation */
#define NB_RANDOM 10    /* Number of randomly generated individuals */
```

Ces constantes définissent respectivement :

* `NB_ELITE` : nombre des meilleurs individus conservés d'une génération à l'autre ;
* `NB_CROSSOVER` : nombre d'individus créés par croisement ;
* `NB_MUTATION` : nombre d'individus créés par mutation ;
* `NB_RANDOM` : nombre d'individus générés aléatoirement.

La somme de ces valeurs doit être égale à `INDIVIDUALS_SIZE`.

## Lancement du projet

Toujours depuis le dossier `src`, lancez l'exécutable :

```bash
./projet_multidisciplinaireS2
```

## Rapport généré

À la fin de l'exécution de l'algorithme, le rapport généré est disponible dans le dossier :

```text
/assets/reports_generated/
```

## Analyse mémoire avec Valgrind

Pour analyser les fuites mémoire et enregistrer le résultat dans un fichier de log :

```bash
valgrind --leak-check=full --log-file=valgrind.log ./projet_multidisciplinaireS2
```
