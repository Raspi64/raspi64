# new Readme

# Raspi 64 - An oldschool IDE for the Raspberry Pi

## Disclaimer

## Included Projects / already Existing Stuff

## installation

## How to use

## Contributing

## Explaining some stuff

## License



# OLD READMEimgui_setup

## Overview

[Dear ImGui](https://github.com/ocornut/imgui) is a window rendering library we will use to create the visual user interface of our raspi64 project.

## Installation on Raspberry Pi

This was tested on Raspberry 3B and 4 with Raspberry Pi OS (32bit) version August 2020.

### 1. Install Media library

ImGui can be docked to several Renderes.

We tested a lot of them, some of them require a huge amount of additional work (which offers again various error sources).
It seems that only [SDL](https://www.libsdl.org) in combination with OpenGL actually works on the Raspberry Pi with a reasonable amount of extra work (actually it's pretty easy to install).

#### Linux / Debian:

_obviously:_

```
$ sudo apt-get update
```

and then

```
$ sudo apt-get install libsdl2-2.0 libsdl2-dev
```

#### macOS

```
$ brew install sdl2
```

### 2. Clone this repo

```
$ git clone https://github.com/Raspi64/imgui_setup
$ cd imgui_setup-main
```

### 3. Compile and build our project

```
$ make
```

### 4. Launch our demo program

```
$ ./imgui_demo
```

### 5. Have fun with our demo!
