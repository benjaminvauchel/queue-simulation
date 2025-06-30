# Simulation et étude d'une file d'attente

Ce projet simule le fonctionnement d'une file d'attente à un guichet, dans le but d'en analyser les performances. Les arrivées des clients suivent une loi exponentielle, et les durées de service sont tirées selon une loi uniforme.

## Objectifs

- Simuler des journées ou périodes complètes avec différentes fréquences d'arrivée (`λ`) et durées de service.
- Étudier les indicateurs de performance : taille moyenne de la file, taux de clients non servis, temps de réponse moyen, etc.
- Générer des fichiers `.csv` pour visualiser les résultats sous forme de courbes.

---

## Structure du code

Le projet est composé de 5 fichiers C (et leurs fichiers d'en-tête associés) :

- `utils.c` : fonctions utilitaires (gestion de l'heure, saisie utilisateur…)
- `lois_probabilite.c` : génération de variables aléatoires (lois exponentielle et uniforme)
- `file.c` : gestion de la file d'attente et des structures `Client` et `File`
- `simulations.c` : fonctions principales de simulation (journée, période, variation de paramètres…)
- `main.c` : point d'entrée du programme, où l'utilisateur peut paramétrer et lancer les simulations

---

##  Utilisation

### Compilation

```bash
make main
```

### Exécution

```bash
./main
```

Des exemples d'utilisation sont disponibles dans main.c.

## Principales fonctions à tester

* `enregistrerJournee` : simule une journée de travail.

* `enregistrerPeriode` : simule plusieurs journées avec enregistrement d'indicateurs à la carte.

* `menuSimulation` : menu interactif de simulation d'une période.

* `enregistrerPeriodeLambda` : variation de la fréquence d'arrivée λ.

* `enregistrerPeriodeMaxsrv` : variation de la durée maximale de service.

## Résultats & Analyse

Le programme permet de générer des fichiers `.csv` analysables.


## Résultats

* **Taille moyenne de la file en fonction de λ :** croissance rapide au-delà d'un certain seuil de saturation.

* **Taux de clients non servis :** augmente fortement lorsque `λ` dépasse une valeur critique (ex. 0.045).

* **Temps de réponse moyen :** dépend fortement de la durée de service maximale et de `λ`.

* **Débit journalier moyen :** suit linéairement la fréquence d'arrivée.

Ces résultats permettent de calibrer les paramètres du système (par exemple la durée de service) pour garantir un service acceptable.

## Remarques

* La simulation avance minute par minute pour une précision simple mais suffisante.

* L'heure est modélisée par un entier représentant le nombre de minutes depuis minuit.

* Les données sont enregistrées selon trois niveaux configurables : par client, par jour, ou sur toute la période.