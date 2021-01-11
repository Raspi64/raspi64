# Raspi 64 - An oldschool IDE for the Raspberry Pi

```
TBD 
hier aus der Dokumentation diese Punkte einfügen:
Aufgabenstellung 
Grund für das Projekt bzw den Gedanken dahinter
```

## Disclaimer

There is no english version at the moment and there isn't any planed. 

Das Programm wurde für die verwendung mit einem Raspberry Pi entwickelt. Es kann auch auf ähnlichen Geräten, Mac, Windows oder anderen Linux Plattformen funktionieren.

## Informationen über das Projekt

Hochschule: [Hochschule Emden/Leer](https://www.hs-emden-leer.de/)

Fachbereich: [Informatik](https://www.hs-emden-leer.de/studierende/fachbereiche/technik/studiengaenge/informatik)

Betreunder Dozent: [Prof. Dr. Carsten Link](http://www.technik-emden.de/~clink/)

Entstehungszeitraum: Wintersemester 2020/21

## Verwendete Projekte 

[Lua](https://www.lua.org/home.html)

[myBasic](https://github.com/paladin-t/my_basic)

[Dear ImGui](https://github.com/ocornut/imgui)

[ImGuiColorTextEdit](https://github.com/BalazsJako/ImGuiColorTextEdit)

Die benötigten Dateien aus den Projekten sind bereits in dieser Repository vorhanden und müssen nicht zusätzlich runtergeladen werden.


## Vorraussetzungen

### 1. Hardware
Getestet wurde das Projekt mit einem Raspberry Pi 3B und Raspberry Pi 4. 
Es kann auch auf älteren beziehungsweise auf ähnlichen Modellen funktionieren allerdings übernehmen wir keine Garantie.

### 2. Betriebssystem
Raspberry Pi OS with Desktop Release 2 Dezember 2020
Es kann auch auf älteren Versionen funktionieren allerdings übernehmen wir keine Garantie.

### 3. Pakete: Zum Ausführen

libsdl2

Die Pakete können über folgenden Befehl installiert werden.

```
$ sudo apt-get update && sudo apt-get install libsdl2
```

### 4. Pakete: Zum compilieren


cmake 

libsdl2-dev

Die Pakete können über folgenden Befehl installiert werden.

```
$ sudo apt-get update && sudo apt-get install cmake libsdl2-dev
```




## Selber compilieren

Nur für erfahrene Nutzer empfohlen die wissen was sie machen. [Hier](https://github.com/Raspi64/raspi64/blob/main/Install.md) findest du eine Detailierte Anleitung wie du es selber compilieren.

## Installation

Du hast zum installieren zwei verschiedene Möglichkeiten. Bei der einen compilierst du das Programm selber, bei der anderen ist es bereits für das beschriebene Raspberry Pi System compiliert.

### 1. Download der neusten Release Version.

Lade dir [hier](https://github.com/Raspi64/raspi64/releases) die neuste Version runter. Die Datei heißt release-x.x.zip. Das x steht im Namen für die Versionsnummer. Wir empfehlen immer die neuste Version.

### 2. Entpacken der Release Version

Gehe mit dem Terminal in den Ordner in dem die .zip Datei liegt und führe den folgenden Befehl zum entpacken aus. Der Befehl erstellt in dem Ordner nun ein Verzeichnis das raspi64 heißt und das Programm beinhaltet. 

```
$ unzip release-0.1.zip -d raspi64/ && cd raspi64
```

### 3. Ausführen des Programmes

Wenn du alle Schritte genau befolgt hast müsste dein Terminal nun in dem entpackten Ordner sein. Zum starten führe den folgenden Befehl aus.

```
$ ./raspi64
```

## Zum Projekt was beitragen / Contributing

This project was just a project during a the winter semester 2020/21 and is our final version. If you find any bugs left in the code that are not mentioned down below or some better ways to solve some functions, create a pull request. We dont guarantee that we will look at them.

Das Projekt war nur eine Aufgabe während des Wintersemesters 2020/21 und ist unsere letzte Version. Solltest du noch fehler finden die nicht bekannt sind oder bessere Lösungsansätze, kannst du gerne sie veröffentlichen. Wir garantieren nicht das wir uns diese Ansehen.



## Bekannte Probleme / Known problems

Die dem Projektteam bekannten Fehler stehen in der [Bugs.md](https://github.com/Raspi64/raspi64/blob/main/Bugs.md), die Datei wird nach bestem Wissen und Gewissen gepflegt. Es besteht auch keine Garantie auf Vollständigkeit.

## License

[MIT](https://choosealicense.com/licenses/mit/)
