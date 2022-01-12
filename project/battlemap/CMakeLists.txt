project(battlemap)

add_executable(${PROJECT_NAME}.elf
        src/main.cpp
        src/BattlemapScene.h
        src/BattlemapScene.cpp
        src/CharacterBase.h
        src/CharacterBase.cpp
        src/Player.h
        src/Player.cpp
        src/Enemy.h
        src/Enemy.cpp
        src/SoldierRed.h src/TileSystemBase.cpp src/TileSystemBase.h src/TileSelectionData.h src/MenuObject.cpp src/MenuObject.h src/MenuManager.cpp src/MenuManager.h)

target_link_libraries(${PROJECT_NAME}.elf gba-sprite-engine)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -v -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.gba
        )
