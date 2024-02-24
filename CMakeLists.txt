cmake_minimum_required(VERSION 3.10)
project(MyApp VERSION 0.1)

# Ensure Fortran code is compiled with the project
enable_language(Fortran)

file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/data)
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION .)

# # SDL2 and friends
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message("-- SDL2 found!")
else ()
    message(FATAL_ERROR "-- SDL2 not found")
endif ()

find_package(SDL2_image REQUIRED)
if (SDL2_image_FOUND)
    message("-- SDL2 image extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 image extension not found")
endif ()

find_package(SDL2_ttf REQUIRED)
if (SDL2_ttf_FOUND)
    message("-- SDL2 ttf extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 ttf extension not found")
endif ()

find_package(SDL2_mixer REQUIRED)
if (SDL2_mixer_FOUND)
    message("-- SDL2 mixer extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 mixer extension not found")
endif ()

find_package(SDL2_net REQUIRED)
if (SDL2_net_FOUND)
    message("-- SDL2 net extension found!")
else ()
    message(FATAL_ERROR "-- SDL2 net extension not found")
endif ()

set(CMAKE_C_FLAGS_RELEASE "-O3 -Wall -Wextra -pedantic -Wshadow")
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -pedantic -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wwrite-strings -Waggregate-return -Wunreachable-code -Wstrict-prototypes -Wstrict-overflow=2 -Wcast-align -Wcast-qual -Wconversion -Wswitch-default -Wswitch-enum")
set(CMAKE_Fortran_FLAGS "-cpp ${CMAKE_Fortran_FLAGS}")

# Include directories
include_directories(
        src
        deps/SDL_FontCache
        deps/stb
)

# Add executable
add_executable(${PROJECT_NAME}
        src/main.c
        src/ran.f95
        deps/SDL_FontCache/SDL_FontCache.c
)

# Link Fortran runtime library if needed
if (CMAKE_Fortran_COMPILER_ID MATCHES "GNU")
    target_link_libraries(${PROJECT_NAME} gfortran)
elseif (CMAKE_Fortran_COMPILER_ID MATCHES "Intel")
    target_link_libraries(${PROJECT_NAME} ifort)
endif ()

# Link required libraries for your project
target_link_libraries(${PROJECT_NAME}
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_ttf
        SDL2_mixer
        SDL2_net
)

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
)