# Final Fantasy Tactics - Clone
This GBA project is powered by W. Groeneveld's C++ GBA engine of which the details can be found via the following link: [GBA sprite engine](https://github.com/wgroeneveld/gba-sprite-engine). **The project's directory** can be found at `project`/`battlemap`. This folder contains the source code, used images, sound effects, and music for this project.

## Project assignment Hasselt University and KU Leuven.
This project is an assignment of the course "software ontwerp in C/C++" (software design in C/C++) 2021-2022. The minimum requirements of the assignment are listed below for convenience.

1. The project is licensed under the MIT license: [view MIT license](https://github.com/i5cP7i/gba-sprite-engine/blob/master/LICENSE).
2. The project started out from the [GBA sprite engine](https://github.com/i5cP7i/gba-sprite-engine) and uses GBA [MODE0](https://github.com/i5cP7i/gba-sprite-engine/blob/624c893aba7d43eb0c8a02df0d82c4ae5601b6cc/engine/src/gba_engine.cpp#L109) and [C++11](https://github.com/i5cP7i/gba-sprite-engine/blob/624c893aba7d43eb0c8a02df0d82c4ae5601b6cc/CMakeLists.txt#L17).
3. This project contains code and uses art taken from or based on external sources. The details can be found in the last chapter which contains all the references. The source code itself also mentions in comments if it is based on external sources.
4. The project should compile. However, this can only be checked by downloading or cloning the repository and compile by using cmake. This project was built using the [CLion IDE](https://www.jetbrains.com/clion/) from Jetbrains. 

## Building the project
Assuming that the user has the (necessary requirements)[https://github.com/wgroeneveld/gba-sprite-engine#compiling-everything] set up, the project can be compiled by using cmake. First, the project has te be cloned or downloaded and extracted to a accessible folder. Open cmd at the project's root folder and create a 'build' folder. Then change directory to the build folder. If on Windows, type:
```
cmake -G "Unix Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND" ./..
```
Next, type `make`. Finally, the gba file `battlemap.gba` can be found at `/build/project/battlemap`.
## References
