# Final Fantasy Tactics - Clone
This GBA project is powered by W. Groeneveld's C++ GBA engine of which the details can be found via the following link: [GBA sprite engine](https://github.com/wgroeneveld/gba-sprite-engine). **The project's directory** can be found at `project`/`battlemap`. This folder contains the source code, used images, sound effects, and music for this project.

## Project assignment Hasselt University and KU Leuven.
This project is an assignment of the course "software ontwerp in C/C++" (software design in C/C++) 2021-2022. The minimum requirements of the assignment are listed below for convenience.

1. The project is licensed under the MIT license: [view MIT license](https://github.com/i5cP7i/gba-sprite-engine/blob/master/LICENSE).
2. The project started out from the [GBA sprite engine](https://github.com/i5cP7i/gba-sprite-engine) and uses GBA [MODE0](https://github.com/i5cP7i/gba-sprite-engine/blob/624c893aba7d43eb0c8a02df0d82c4ae5601b6cc/engine/src/gba_engine.cpp#L109) and [C++11](https://github.com/i5cP7i/gba-sprite-engine/blob/624c893aba7d43eb0c8a02df0d82c4ae5601b6cc/CMakeLists.txt#L17).
3. This project contains code and art taken from or based on external sources. The details can be found in the last chapter which contains all the references. The source code itself also mentions in comments if it is based on external sources.
4. The project should compile. However, this can only be checked by downloading or cloning the repository and compile by using cmake. This project was built using the [CLion IDE](https://www.jetbrains.com/clion/) from Jetbrains. 
5. The ReadME contains a short functional description of the game. This description is given below.
6. The ReadME also contains a sketch of the OO model specific to the project supported with a description.

## Building the project
Assuming that the user has the (necessary requirements)[https://github.com/wgroeneveld/gba-sprite-engine#compiling-everything] set up, the project can be compiled by using cmake. First, the project has te be cloned or downloaded and extracted to a accessible folder. Open cmd at the project's root folder and create a 'build' folder. Then change directory to the build folder. If on Windows, type:
```
cmake -G "Unix Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ./..
```
Next, type `make`. Finally, the gba file `battlemap.gba` can be found at `/build/project/battlemap`.

## Functional Description

This is a Final Fantasy Tactics clone based on the original Final Fantasy Tactics Advance (Square Enix, 2003) which is a turn-based role-playing strategy game for the gameboy advance. This clone features a basic implementation of the battle system and can potentially be expanded with more maps and abilities. The following description elaborates on the current implementation and its functionality.

### Main Menu Scene
![MainMenuScene](https://user-images.githubusercontent.com/48355782/151185305-941d5fbc-add6-4130-8131-13c649384e61.gif)

At startup, the main menu is loaded. When the user presses the "A" button, the game transitions into the battlemap scene.

### Battlemap Scene

This is the battlemap scene where the gameplay takes place. The battle is located on a grass field where two player characters duel with melee combat. The combat is turn-based, so each player can move and act only once each turn. Each player can either move, act, or wait. After a move and an action, the player has to wait and hand over the turn to the other player. After moving (or acting), the player can still act (or move) or wait. Moving and acting is final and cannot be undone. The available actions are "attack" and "items" which is further explained below. The game ends if either player has zero or less health points, which is displayed while moving and attacking in the corners of the screen.

![battlemapScene](https://user-images.githubusercontent.com/48355782/151186224-2c791616-813c-4523-bbd4-8bea28aa2c0d.png)

<details>
<summary>Start of the Battle</summary>

At the beginning of the battlemap scene, the two characters and the battlemap background are shown. If the user presses the "Start" button, the game begins.
  
![image](https://user-images.githubusercontent.com/48355782/151186751-c9f166b3-9756-4b03-b341-08edaa047d09.png)

The game has started. To perform an action or move, press the "A" button. This opens the menu system. The player can now choose to move, act, or wait. Pressing the "B" button closes the menu system. The menu system can be navigated top to bottom using the "down" or "up" arrow keys.

![image](https://user-images.githubusercontent.com/48355782/151192125-2978cd34-34c1-47a7-ad2a-882fa1e1bda8.png)
<p><em>The menu system</em></p>

</details>

<details>
<summary>Player Move and Wait</summary>

The player has to select a tile using the "arrow keys" within the boundaries of the map and his move radius. The move radius dictates how many tiles the player can cross and thus determines how far the player can move in each direction. There are four directions in which the player can move: Northeast, southeast, southwest, and Northwest. The player cannot act to the "right" (east, positive x direction), "left" (west, negative x direction), "up" (north, negative y direction), or "down" (south, positive y direction). The player moves instantaneously to the designated tile after pressing "A", if it is a valid tile to move on. if "B" is pressed, the player returns to the menu.
  
![image](https://user-images.githubusercontent.com/48355782/151189692-63f98f64-4caf-41f4-9558-63709706b181.png)
<p><em>Player "Blue" selects a tile within his move radius (left), and moves to it (right).</em></p>

After moving, the player can still act or wait. For instance, the player could still choose to heal himself although he is at full health. If the player decides to wait, then his turn will be handed over to the other player. When the "wait" option is selected, the player selects a direction using the arrow keys. After pressing "A", the player's turn has ended.

![image](https://user-images.githubusercontent.com/48355782/151192564-055b5d02-d9d7-4214-95df-1ba18963342c.png)
<p><em>Player "Blue" is ending his turn and can select a direction.</em></p>

It is now player Red's  turn. This character is identical the first player, but he has less health, and more strength. He can move in a similar fashion. The images below show some invalid move selections.

![image](https://user-images.githubusercontent.com/48355782/151190288-cafa88ec-8247-4c60-bf6b-682bc17d9257.png)
<p><em>Player "Red" selects a tile out of his move radius and cannot move to it.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151191140-a69ad339-548b-4dc4-a5a0-a20cc58568f9.png)
<p><em>Player "Red" selects a tile within his move radius, but he is currently occupying it.</em></p>

If the players are next to each other i.e., one tile away in the direction in which they can act, then they can enter combat. The next section discusses player actions.

![image](https://user-images.githubusercontent.com/48355782/151193388-80de4ab7-14b8-41d4-accd-0aaecbd50f3e.png)
<p><em>The players are now within the attack range and can enter combat.</em></p>
</details>

<details>
<summary>Player Action</summary>

The "Action" menu contains two options: "Attack" and "Items". Press the "up" or "down" arrow key to navigate to the action menu, and press "A" to open it.

![image](https://user-images.githubusercontent.com/48355782/151193855-2d3df1d5-a288-4ff0-beea-d2fff1cf5fcc.png)
<p><em>Navigation to the action menu.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151194207-432c99be-d858-4641-8637-2678e9104685.png)
<p><em>The action menu contents.</em></p>
  
The player can choose to attack a enemy standing in the direction in which he can act, which is northeast, southeast, southwest, or northwest. He cannot attack himself or attack a tile which is not occupied by the enemy. Neither can the player attack outside his attack range, which is a radius of one tile.

![image](https://user-images.githubusercontent.com/48355782/151194782-b08b6fef-819e-4f55-8959-32ed6a56bc1b.png)
<p><em>The player cannot attack himself.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151194977-25b607ca-0992-4cdc-b2ed-94957d7929dc.png)
<p><em>The player selects a tile within the attack range, but there is no enemy occopying it.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151195189-a1c9bac2-ac8b-41b2-9569-944adc172023.png)
<p><em>The player selects a tile outside his attack radius.</em></p>
  
When the player chooses to attack the enemy, he will deal damage based on his strength parameter. The blue player has a strength rating of 20, and the red player has a strength rating of 25.

![image](https://user-images.githubusercontent.com/48355782/151197447-e8d8fda8-394b-454d-8416-52a1cd934403.png)
<p><em>Player "Blue" attacks Player "Red".</em></p>

The red player is damaged. He can now choose to heal himself with a potion or strike back, move back, and wait or do nothing and wait. Or he could choose to only move and wait. This is up to the player. 

![image](https://user-images.githubusercontent.com/48355782/151197817-8a4b1751-73df-471f-ad38-e1b3353c0ff2.png)
<p><em>Player "Red" is damaged and chooses to heal himself.</em></p>

Note that each player who chooses to heal, can also heal the enemy!

![image](https://user-images.githubusercontent.com/48355782/151198031-26328775-5a15-4b6f-9257-846390f4487f.png)
<p><em>Player "Red" highlights the enemy player's tile.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151198395-3564890e-7580-4941-9e00-20ad9867ece2.png)
<p><em>Player "Red" accidentally heals the enemy.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151198695-0d527a0f-5ec8-484f-a06c-6e889d72f139.png)
<p><em>Player "Red" heals himself.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151199091-f9658734-1010-44c9-a294-2a5e42c47f27.png)
<p><em>Player "Red" tries to escape.</em></p>
</details>

<details>
<summary>Player Death</summary>

When either one of the players reaches zero health or less loses the game. This also ends the game and the game transitions the end game menu.
  
![image](https://user-images.githubusercontent.com/48355782/151199741-b0778d35-627e-432a-a47f-e9b31a470861.png)
<p><em>Player "Red" has zero health and loses the game.</em></p>
</details>

<details>
<summary>Victory Sequence and menu</summary>

The winning player is presented and the player has now the option to retry or quit to the main menu. If retry is selected, then the battle resets and can be played again. If quit is selected, the game transitions to the main menu scene.
  
![image](https://user-images.githubusercontent.com/48355782/151200154-d1c7dede-6d25-47e7-bb95-c9a1b2235d1d.png)
<p><em>Player "Blue" wins.</em></p>

![endgamequit](https://user-images.githubusercontent.com/48355782/151202626-5a583ca9-7579-476c-b4c5-1c9414370653.gif)
<p><em>Select quit to return to the main menu.</em></p>

![endgameretry](https://user-images.githubusercontent.com/48355782/151203665-e8021386-ae8e-4597-b8c8-01337c039639.gif)
<p><em>Select retry to play again.</em></p>
</details>
  
## The Object Oriented Model 

![domainmodel_fftclone drawio](https://user-images.githubusercontent.com/48355782/151258740-d4913a92-8f0f-4ed1-be3f-0af2f20aac32.png)
<p><em>Domain Model</em></p>

The figure above shows a simplified overview of the object oriented (OO) model specific to the game. The startscene is loaded at startup, which is the main menu scene. It contains a menumanager and menuobject object to facilitate the player with a interactive menu. Currently, the player can only start the game. The scene simply polls for the start key, and if pressed, the engine transitions into the battlemap scene. 
This scene comprises of a menuobject, menumanager, tilesystem, characters, and a variety of member variables and methods. The menuobject and menumanager constitute the menu system. The menusystem controls the state of the game and thus controls the gameplay. There are two characters: a player and an enemy. Both characters are inherited from the characterbase class. 
The characterbase class contains all member variables and methods specific to the character such as, Animatewalking(), AnimateAttack(), GetHealth(), etc. Both the player and enemy are thus characters, but they have different stats and use different spritedata. Therefore, a characterbase class is required to differentiate the players. The characterbase class builds the characters with their respective spritedata. Each character also has a sword which is visible while attacking. This sword is an object from the object class and also belongs to the characterbase, though it would be better to link the weapon to the playerCharacter, because NPCs do not necessarily possess a weapon. The characterbase class also has a tilesystem linked to it, because each character also occupies a tile on the map. 
This tilesystem is a class that maps the GBA x,y coordinates to a new coordinatesystem: the cartesian coordinate system which is relative with respect to the isometric directions: Southwest (positive cartesian x) and southeast (positive cartesian y). It also contains the sprites to highlight a tile on the map.
These elements form the core of the battlemap scene, where the action takes place. The user can move, act, and wait, until one of the characters reaches a health value below zero, which ends the game. If the game has ended, the user can select "retry" or "quit". If retry is selected, the game is reset and the gameplay restarts after pressing "start". If quit is selected, then the game engine returns to the main menu.

## References

The "key pressed" and "key released" functionality is based on the code in this [link](https://jamiedstewart.github.io/gba%20dev/2019/03/21/GBA-Dev-Input-Handling.html).
The Menu system is based on a tree datastructure which is adapted from a programmer on youtube, which can be found [here](https://youtu.be/jde1Jq5dF0E).







