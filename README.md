# Raspi 64 - An oldschool IDE for the Raspberry Pi

Das Projekt Raspi 64 enstand im Rahmen der Projektgruppe des [Studiengangs Informatik](https://www.hs-emden-leer.de/studierende/fachbereiche/technik/studiengaenge/informatik) der [Hochschule Emden/Leer](https://www.hs-emden-leer.de/). Das Projekt wurde von [Prof. Dr. Carsten Link](http://www.technik-emden.de/~clink/) unter dem Titel "Entwicklung einer Programmierumgebung für Anfänger auf dem Raspberry Pi" ausgeschrieben. Das Projekt wurde mit diesen Stichworten beschrieben:

```
- Anschließen, einschalten, loslegen
- sehr einfache Programmiersprache
- Programmierung des Computers, nicht irgendwelcher Bibliotheken oder Betriebssystem-API
```

Als Programmiersprachen die der Nutzer am Ende in der Entwicklungsumgebung verwenden kann stand direkt zu Beginn das verwenden von [Basic](https://en.wikipedia.org/wiki/BASIC) fest, [Lua](https://en.wikipedia.org/wiki/Lua) war am Anfang nicht mehr als ein Gedanke der in einer Liste von möglichen Teams für das Projekt stand. Das Team hat sich dann dafür entschieden die Sprache ebenfalls zu unterstützen.

Als Aufgabe wurde dann Festgelegt das, das Team eine Eigene Entwicklungsumgebung für den Rasberry Pi entwickelt, nach dem Vorbild des [Commodore 64](https://en.wikipedia.org/wiki/Commodore_64). Der Nutzer sollte mit der Entwciklungsumgebung auch für die Sprachen entsprechende Dokumentaion mit ausgeliefert bekommen. Die Entwicklungsumgebung sollte allerdings mehr als nur ein Textfeld beinhalten in dem Code geschrieben werden kann. Als Inspiration für den groben Aufbau der [GUI](https://en.wikipedia.org/wiki/Graphical_user_interface) wurde dann das existierende Project [Quite Basic](http://www.quitebasic.com/) genommen. 


## Disclaimer

There is no english version at the moment and there isn't any planed. 

Das Programm wurde für die Verwendung mit einem Raspberry Pi entwickelt. Es kann auch auf ähnlichen Geräten wie Mac, Windows oder anderen Linux Plattformen funktionieren.

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

Es kann auch auf älteren beziehungsweise auf ähnlichen Modellen funktionieren allerdings übernehmen wir hierfür keine Garantie.

### 2. Betriebssystem
[Raspberry Pi OS with Desktop Release 2 Dezember 2020](https://downloads.raspberrypi.org/raspios_armhf/images/raspios_armhf-2020-12-04/2020-12-02-raspios-buster-armhf.zip)

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

Das selber Compilieren wird ausschließlich erfahrenen Nutzern empfohlen. [Hier](https://github.com/Raspi64/raspi64/blob/main/Install.md) findest du eine detailierte Anleitung wie du das Projekt selber compilieren kannst.

## Installation

Du hast zum Installieren zwei verschiedene Möglichkeiten. Bei der einen compilierst du das Programm selber, bei der anderen ist es bereits für das beschriebene Raspberry Pi System compiliert.

### 1. Download der neusten Release Version.

Lade dir [hier](https://github.com/Raspi64/raspi64/releases) die neuste Version herunter. Die Datei heißt release-x.x.zip. Das x im Namen steht für die Versionsnummer. Wir empfehlen die neuste Version.

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

This project was just a project during a the winter semester 2020/21 and is our final version. If you find any bugs left in the code that are not mentioned down below or some better ways to solve some functions, create a pull request. We don't guarantee that we will look at them.

Das Projekt war nur eine Aufgabe während des Wintersemesters 2020/21 und ist unsere letzte Version. Solltest du noch fehler finden die nicht bekannt sind oder bessere Lösungsansätze, kannst du gerne sie veröffentlichen. Wir garantieren nicht das wir uns diese Ansehen.



## Bekannte Probleme / Known problems

Die dem Projektteam bekannten Fehler stehen in der [Bugs.md](https://github.com/Raspi64/raspi64/blob/main/Bugs.md), die Datei wird nach bestem Wissen und Gewissen gepflegt. Es besteht keine Garantie auf Vollständigkeit.

## License

[MIT](https://choosealicense.com/licenses/mit/)
