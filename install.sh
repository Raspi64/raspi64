#!/usr/bin/env bash

git clone https://github.com/Raspi64/imgui_setup.git
cd imgui_setup
sudo apt install cmake libsdl2-dev
./build.sh
