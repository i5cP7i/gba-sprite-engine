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

### Main Menu Scene
![MainMenuScene](https://user-images.githubusercontent.com/48355782/151185305-941d5fbc-add6-4130-8131-13c649384e61.gif)

### Battlemap Scene
![battlemapScene](https://user-images.githubusercontent.com/48355782/151186224-2c791616-813c-4523-bbd4-8bea28aa2c0d.png)

#### Start of the Battle
![image](https://user-images.githubusercontent.com/48355782/151186751-c9f166b3-9756-4b03-b341-08edaa047d09.png)

![image](https://user-images.githubusercontent.com/48355782/151192125-2978cd34-34c1-47a7-ad2a-882fa1e1bda8.png)
<p><em>The menu system</em></p>

#### Player Move
![image](https://user-images.githubusercontent.com/48355782/151189692-63f98f64-4caf-41f4-9558-63709706b181.png)
<p><em>Player "Blue" selects a tile within his move radius (left), and moves to it (right).</em></p>

![image](https://user-images.githubusercontent.com/48355782/151192564-055b5d02-d9d7-4214-95df-1ba18963342c.png)
<p><em>Player "Blue" is ending his turn and can select a direction.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151190288-cafa88ec-8247-4c60-bf6b-682bc17d9257.png)
<p><em>Player "Red" selects a tile out of his move radius and cannot move to it.</em></p>

![image](https://user-images.githubusercontent.com/48355782/151191140-a69ad339-548b-4dc4-a5a0-a20cc58568f9.png)
<p><em>Player "Red" selects a within his move radius, but he is currently occupying it.</em></p>

## The Object Oriented Model 


## References



