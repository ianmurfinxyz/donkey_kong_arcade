cmake_minimum_required(VERSION 3.20)
project(donkey_kong_1981 VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
        src/core/main.cc
        src/core/log.cc
        src/core/input.cc
        src/core/rand.cc
        src/assets/sprites.cc
        src/game/play.cc
        src/core/animation.cc
        src/core/cutscene.cc
        src/game/game.cc
        src/game/intermission.cc
        src/game/world.cc
        src/game/constants.cc
        src/game/stages/classic_barrels.cc
        src/game/stages/classic_factory.cc
        src/game/stages/classic_elevators.cc
        src/game/stages/classic_rivets.cc
        src/assets/cutscenes.cc src/assets/fonts.cc src/assets/fonts.hh src/hud/hud.hh src/hud/hud.cc src/assets/sounds.hh src/assets/sounds.cc src/hiscores/hiscores.cc src/hiscores/hiscores.hh src/menu/menu.cc src/menu/menu.hh src/title/title.cc src/title/title.hh)

set(ALLEGRO_LIBRARIES
        -lallegro
        -lallegro_main
        -lallegro_font
        -lallegro_ttf
        -lallegro_audio
        -lallegro_primitives
        -lallegro_image
        -lallegro_audio
        -lallegro_acodec)

add_executable(donkey_kong_1981 ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(donkey_kong_1981 ${ALLEGRO_LIBRARIES})
