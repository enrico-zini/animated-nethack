#!/bin/bash
set -e

gcc main.c AnimationController.c AStar.c -lGL -lGLU -lglut -o game.out -Wall -Wextra -Wpedantic
./game.out
