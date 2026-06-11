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

Pour modifier le nombre d'individus par génération, changez la valeur de :

```c
INDIVIDUALS_SIZE
```

Pour modifier le nombre total de générations, changez la valeur de :

```c
NB_GENERATIONS
```

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
