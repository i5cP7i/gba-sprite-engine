# Final Fantasy Tactics - Clone
This GBA project is powered by W. Groeneveld's C++ GBA engine of which the details can be found via the following link: [GBA sprite engine](https://github.com/wgroeneveld/gba-sprite-engine). **The project's directory** can be found at `project`/`battlemap`. This folder contains the source code, used images, sound effects, and music for this project.

## Project assignment Hasselt University and KU Leuven.
This project is an assignment of the course "software ontwerp in C/C++" (software design in C/C++) 2021-2022. The minimum requirements of the assignment are listed below for convenience.

1. The project is licensed under the MIT license: [view MIT license](https://github.com/i5cP7i/gba-sprite-engine/blob/master/LICENSE).
2. The project started out from the [GBA sprite engine](https://github.com/i5cP7i/gba-sprite-engine) and uses GBA [MODE0](https://github.com/i5cP7i/gba-sprite-engine/blob/624c893aba7d43eb0c8a02df0d82c4ae5601b6cc/engine/src/gba_engine.cpp#L109) and [C++11](https://github.com/i5cP7i/gba-sprite-engine/blob/624c893aba7d43eb0c8a02df0d82c4ae5601b6cc/CMakeLists.txt#L17).
3. This project contains code and uses art taken from or based on external sources. The details can be found in the last chapter which contains all the references. The source code itself also mentions in comments if it is based on external sources.
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

At the beginning of the battlemap scene, the two characters and the battlemap background are shown. If the user presses the start button, the game begins.
  
![image](https://user-images.githubusercontent.com/48355782/151186751-c9f166b3-9756-4b03-b341-08edaa047d09.png)



![image](https://user-images.githubusercontent.com/48355782/151192125-2978cd34-34c1-47a7-ad2a-882fa1e1bda8.png)
<p><em>The menu system</em></p>

</details>

<details>
<summary>Player Move and Wait</summary>

![image](https://user-images.githubusercontent.com/48355782/151189692-63f98f64-4caf-41f4-9558-63709706b181.png)
<p><em>Player "Blue" selects a tile within his move radius (left), and moves to it (right).</em></p>

![image](https://user-images.githubusercontent.com/48355782/151192564-055b5d02-d9d7-4214-95df-1ba18963342c.png)
<p><em>Player "Blue" is ending his turn and can select a direction.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151190288-cafa88ec-8247-4c60-bf6b-682bc17d9257.png)
<p><em>Player "Red" selects a tile out of his move radius and cannot move to it.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151191140-a69ad339-548b-4dc4-a5a0-a20cc58568f9.png)
<p><em>Player "Red" selects a tile within his move radius, but he is currently occupying it.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151193388-80de4ab7-14b8-41d4-accd-0aaecbd50f3e.png)
<p><em>The players are now within the attack range and can enter combat.</em></p>
</details>

<details>
<summary>Player Action</summary>

![image](https://user-images.githubusercontent.com/48355782/151193855-2d3df1d5-a288-4ff0-beea-d2fff1cf5fcc.png)
<p><em>Navigation to the action menu.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151194207-432c99be-d858-4641-8637-2678e9104685.png)
<p><em>The action menu contents.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151194782-b08b6fef-819e-4f55-8959-32ed6a56bc1b.png)
<p><em>The player cannot attack himself.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151194977-25b607ca-0992-4cdc-b2ed-94957d7929dc.png)
<p><em>The player selects a tile within the attack range, but there is no enemy occopying it.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151195189-a1c9bac2-ac8b-41b2-9569-944adc172023.png)
<p><em>The player selects a tile outside his attack radius.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151197447-e8d8fda8-394b-454d-8416-52a1cd934403.png)
<p><em>Player "Blue" attacks Player "Red".</em></p>

![image](https://user-images.githubusercontent.com/48355782/151197817-8a4b1751-73df-471f-ad38-e1b3353c0ff2.png)
<p><em>Player "Red" is damaged and chooses to heal himself.</em></p>

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

![image](https://user-images.githubusercontent.com/48355782/151199741-b0778d35-627e-432a-a47f-e9b31a470861.png)
<p><em>Player "Red" has zero health and loses the game.</em></p>
</details>

<details>
<summary>Victory Sequence and menu</summary>

![image](https://user-images.githubusercontent.com/48355782/151200154-d1c7dede-6d25-47e7-bb95-c9a1b2235d1d.png)
<p><em>Player "Blue" wins.</em></p>

![endgamequit](https://user-images.githubusercontent.com/48355782/151202626-5a583ca9-7579-476c-b4c5-1c9414370653.gif)
<p><em>Select quit to return to the main menu.</em></p>

![endgameretry](https://user-images.githubusercontent.com/48355782/151203665-e8021386-ae8e-4597-b8c8-01337c039639.gif)
<p><em>Select retry to play again.</em></p>
</details>
  
## The Object Oriented Model 


## References







