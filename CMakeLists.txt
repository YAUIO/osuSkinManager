cmake_minimum_required(VERSION 3.28)
project(osuSkinManager)
include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS FALSE)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt
        GIT_TAG 10.2.1
)

FetchContent_MakeAvailable(fmt)

FETCHCONTENT_DECLARE(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML
        GIT_TAG 2.6.1
)

FETCHCONTENT_MAKEAVAILABLE(SFML)

add_executable(osuSkinManager main.cpp
        Files.cpp
        Files.h
        GUI.cpp
        GUI.h
)

TARGET_LINK_LIBRARIES(osuSkinManager fmt)
target_link_libraries(osuSkinManager sfml-graphics)