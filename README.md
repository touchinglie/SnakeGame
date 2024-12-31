# Snake Game

The Snake Game is completely based on the repository of [csnake](https://github.com/Cutotopo/csnake)
It's an redevelopment of the original Snake Game in C using the GTK library.
Anyway it just an implementation for my schoolwork, more development will be done in original repository[csnake](https://github.com/Cutotopo/csnake).

## Advanced Features

* Diffculty level. It has 2 difficulty levels, easy and hard.
* Now it has Stop and Continue! Just press the J in your keyboard during the game.
* Variable game speed. Now it has an nonlinear model to change the speed of the game, it depends on the score and the difficulty level.
* More skins due to the diffculty level.
* Restart button. You can restart the game without closing the window and clicking another time now.
* Sound effects. The game has sound effects for the game over, the apple eating, and the bgm.

## The issues

* The game can't be started if the GTK libraries are not installed.
* It was only working on Windows due to the Winapi.

-----------------------------------------------------------------
The README.md belongs to the original repository of [csnake](https://github.com/Cutotopo/csnake) below here.

# csnake
Simple Snake game written in C using GTK

## Flags
csnake supports the following flags:
| Flag             | Short flag | Description                                             | Expected input   |
| ---------------- | ---------- | ------------------------------------------------------- | ---------------- |
| --refreshTimeout | -rt        | Set game tick interval in milliseconds (default is 200) | Positive integer |
| --help           | -h         | Show help message                                       |                  |

## Development
If GTK libraries are installed on your system, you may be able to build this software using:
```
gcc $(pkg-config --cflags gtk4) csnake.c -o csnake -lm $(pkg-config --libs gtk4)
```
