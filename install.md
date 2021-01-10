# Installationsaleitung des Programmes

## Disclaimer
Das Programm wurde für die verwendung mit einem Raspberry Pi entwickelt. Es kann auch auf ähnlichen Geräten, Mac, Windows oder anderen Linux Plattformen funktionieren. 

In Bezug auf die Installation wird hier nur die vom Entwicklungsteam verwendete und verifizierte Vorgehensweise gezeigt. Der Abschnitt Quickstart beinhaltet alle nötigen Befehle in der richtigen Reihenfolge. Genaure Erklärungen stehen weiter unten.

## Quickstart

Nur verwenden wenn du weißt was du tust. Nur für fortgeschrittene Nutzer empfohlen. Die Vorraussetzungen stehen weiter unten.

```
$ sudo apt-get update
$ sudo apt-get install cmake libsdl2-dev
$ git clone https://github.com/Raspi64/raspi64.git
$ ./raspi64/build.sh
```



## Vorraussetzungen

### 1. Hardware
Getestet wurde das Projekt mit einem Raspberry Pi 3B und Raspberry Pi 4. 
Es kann auch auf älteren beziehungsweise auf ähnlichen Modellen funktionieren allerdings übernehmen wir keine Garantie.

### 2. Betriebssystem
Raspberry Pi OS with Desktop Release 2 Dezember 2020
Es kann auch auf älteren Versionen funktionieren allerdings übernehmen wir keine Garantie.

### 3. Pakete
cmake 

libsdl2-dev

Die Pakete können über folgenden Befehl installiert werden.

```
$ sudo apt-get update && sudo apt-get install cmake libsdl2-dev
```

## Eigentliche installation des Programmes

Es wird davon ausgegangen das die Vorraussetzungen erfüllt werden.

### 1. Klonen des Projektes

Das Projekt wird mit allen benötigten Komponennten und älteren Versionen heruntergeladen und im aktuellen Ordner gespeichert gespeichert.

```
$ git clone https://github.com/Raspi64/raspi64.git
```

### 2. Build.sh

Das Programm wird nun kompiliert und ausführbar gemacht. Dies kann etwas dauern. Bitte drücke keine Tasten um Fehler zu verhindern. Die Warnings sind Teil des Vorganges und normal, du musst dir keine Sorgen machen deswegen.

```
$ ./raspi64/build.sh
```

### 3. Starten des Programmes
Das Programm kann nun mit diesem Befehl gestartet werden. Alles zur Verwendung des Programmes kannst du dir im Hilfesystem ansehen.


```
$ ./raspi64/imgui_setup
```
