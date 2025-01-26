cmake_minimum_required(VERSION 3.5.0)
project(SpaceFighter VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

include_directories(
    thirdparty/sdl2/include
    thirdparty/sdl2_mixer/include
    thirdparty/sdl2_image/include
    thirdparty/sdl2_ttf/include

    thirdparty/json/include

    headers
)

link_directories(
    thirdparty/sdl2/lib/x64
    thirdparty/sdl2_mixer/lib/x64
    thirdparty/sdl2_image/lib/x64
    thirdparty/sdl2_ttf/lib/x64
)


add_executable(SpaceFighter 
    src/main.cpp   
    src/Game.cpp 

    src/SceneMenu.cpp
    src/SceneGaming.cpp
    src/SceneEnd.cpp

    src/SceneManager.cpp 
    src/EntityManager.cpp
    src/ResourceManager.cpp
    src/CollisionManager.cpp
    src/AudioManager.cpp

    src/Timer.cpp
    src/Camera.cpp
    src/Atlas.cpp
    src/Animation.cpp
    src/CollisionBox.cpp

    src/Entity.cpp
    src/Player.cpp
    src/Enemy.cpp

    src/Bullet.cpp
    src/BulletPlayer.cpp
    src/BulletEnemy.cpp

    src/DropItem.cpp

    src/Background.cpp

    resources/ico/my_icon.rc
)

target_link_libraries(SpaceFighter SDL2 SDL2main SDL2_mixer SDL2_image SDL2_ttf)

set_target_properties(SpaceFighter PROPERTIES
    WIN32_EXECUTABLE TRUE
)