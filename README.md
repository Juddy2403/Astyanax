# Astyanax-Inspired C++ Game

Welcome to my simple C++ game project inspired by the classic game [**Astyanax**](https://en.wikipedia.org/wiki/The_Astyanax)! This project was developed during my second semester of school as part of a programming assignment. It was a great opportunity to practice and improve my C++ skills, especially in encapsulation and basic game development concepts. While the game is not a perfect replica of Astyanax, it incorporates many of its core mechanics with some simplifications and changes to make it easier to implement and play.

---

## Table of Contents
1. [Overview](#overview)
2. [Features](#features)
3. [Controls](#controls)
4. [Differences from the Original Game](#differences-from-the-original-game)
5. [Technical Details](#technical-details)
6. [How to Play](#how-to-play)
7. [Releases](#releases)
8. [Credits](#credits)

---

## Overview
This project is a 2D side-scrolling action game where the player controls a hero who must defeat enemies and clear a level. The game was built using a simple C++ game engine provided by my teacher. It features basic gameplay mechanics such as movement, attacking, power-ups, and enemy interactions. The game is not polished or optimized for professional standards, but it served as a valuable learning experience in game development and C++ programming.

---

## Features
- **Single Level Gameplay**: The game consists of one level. Upon dying, the level resets. Upon winning, an image is displayed indicating the first round has been cleared, after which the level resets.
- **Enemy System**: Enemies can drop random pick-ups when defeated. The boss is a stronger version of the Skeleton enemy with a different-colored skin.
- **Power-Ups and Weapon Upgrades**: Players can collect power-ups dropped by enemies and weapon upgrades dropped by pillars.
- **SP Regeneration**: Special Points (SP) regenerate over time, allowing players to use power-ups more frequently.
- **Simplified Memory Management**: Enemy instances are not freed and reallocated upon respawning. Instead, their positions and stats are reset, and they are reactivated after certain conditions are met.
- **Attacking While Moving**: Players can attack while crouching or jumping.

---

## Controls
- **Left Arrow**: Move left
- **Right Arrow**: Move right
- **Up Arrow**: Jump
- **Down Arrow**: Crouch
- **Z Key**: Normal attack
- **X Key**: Use power-up
- **Left Shift Key**: Switch between power-ups
- **Esc Key**: Pause the game

---

## Differences from the Original Game
- Only one level is available.
- The boss is replaced with a stronger version of the Skeleton enemy.
- SP regenerates over time.
- Enemies drop random pick-ups when killed.
- Pillars always drop weapon upgrades.
- Simplified enemy respawning system (no memory reallocation).

---

## Technical Details
- **Language**: C++
- **Game Engine**: A simple C++ game engine provided by my teacher.
- **Graphics**: Original Astyanax sprites were used.
- **Collision Detection**: A `.svg` file was used for level collision.
- **Memory Management**: Enemy instances are reused by resetting their positions and stats instead of reallocating memory.

---

## How to Play
1. Download the latest build from the [Releases](#releases) section.
2. Extract the files and run the executable.
3. Use the controls listed above to play the game.
4. Defeat enemies, collect power-ups, and clear the level to win!

---

## Releases
You can find the latest build of the game in the [Releases](https://github.com/Juddy2403/Astyanax/releases) section. 

---

## Credits
- [**Original Astyanax Sprites**](https://www.spriters-resource.com/nes/astyanax/): Used for the game's graphics.
- [**Original Astyanax Audio**](https://downloads.khinsider.com/game-soundtracks/album/astyanax-nes): Used to bring the game to life.
- [**Koen Samyn**](https://www.linkedin.com/in/koen-samyn-1652235/): Provided the simple C++ game engine and guidance. 
- [**Me**](https://www.linkedin.com/in/ioana-raileanu-147725252/): Developed the game as part of a school project.
- Original NES game can be played [here](https://www.retrogames.cz/play_1491-NES.php)

---

Thank you for checking out my project! Feel free to explore the code, play the game, and provide feedback. This was a great learning experience, and I hope to continue improving my skills in game development and programming. Enjoy! 🎮
