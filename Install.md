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
$ cd rapsi64
$ ./build.sh
```

## Installation des Programmes

Es wird davon ausgegangen das die Vorraussetzungen erfüllt werden.

### 1. Klonen des Projektes

Das Projekt wird mit allen benötigten Komponennten und älteren Versionen heruntergeladen und im aktuellen Ordner gespeichert gespeichert.

```
$ git clone https://github.com/Raspi64/raspi64.git
```

### 2. Build.sh

Das Programm wird nun kompiliert und ausführbar gemacht. Dies kann etwas dauern. Bitte drücke keine Tasten um Fehler zu verhindern. Die Warnings sind Teil des Vorganges und normal, du musst dir keine Sorgen machen deswegen. 

```
$ cd rapsi64
$ ./build.sh
```

### 3. Starten des Programmes
Das Programm kann nun mit diesem Befehl gestartet werden. Alles zur Verwendung des Programmes kannst du dir im Hilfesystem ansehen.


```
$ ./imgui_setup
```
