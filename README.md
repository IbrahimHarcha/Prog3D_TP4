# Template de TP


## Dépendances

- cmake (Outil pour créer des projets cross-plateforme)
- glm (Bibliothèque de mathématiques adaptées à OpenGL)
- glut
- assimp (Bibliothèque d'import de modèles 3D)

Sur Ubuntu (avec les droits administateurs):
```
apt install cmake freeglut3-dev libassimp-dev libglm-dev
```

Si vous n'avez pas les droits il vous faut compiler les deux sous-modules dans le dossier "thirdparties".
```
git submodule update --init --recursive
```
Normalement le script de compilation s'occupe de compiler les deux modules une fois que le code est téléchargé.

## Compilation

### Automatique

```
sh build.sh
```

### A la main
Pour créer le makefile :
```
template/$ mkdir build
template/$ cd build
template/build$ cmake ..
```

Pour tout compiler :
```
template/build$ make
```

Pour compiler le tp seulement :
```
template/build$ make tp
```

## Exécution

### Par le script `run.sh`
```
sh run.sh ./data/bunny.obj
```

```
./build/tp ./data/bunny.obj # utilise le modèle bunny.obj du dossier `data`
./build/tp ./data/ToyCar.glb # utilise le modèle ToyCar.glb du dossier `data`
./build/tp <anyformat> # ce que vous voulez comme format suppporté par Assimp
```


