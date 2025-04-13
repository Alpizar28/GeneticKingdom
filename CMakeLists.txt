cmake_minimum_required(VERSION 3.10)
project(GeneticKingdom)

set(CMAKE_CXX_STANDARD 17)

# Incluir todos los directorios de headers explícitamente
include_directories(
    include
    src
    src/core
    src/map
    src/towers
    src/enemies
    src/ai
    src/ui
)

# Buscar todos los .cpp del proyecto (en cualquier subcarpeta)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Buscar SFML y enlazar
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(GeneticKingdom ${SOURCES})
target_link_libraries(GeneticKingdom sfml-graphics sfml-window sfml-system)
