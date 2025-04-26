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
# Copiar assets al directorio de build
file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
# Buscar todos los .cpp del proyecto (en cualquier subcarpeta)
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" 
    "src/map/Map.cpp"  # Añade esta línea explícitamente
)
# Buscar SFML y enlazar
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(GeneticKingdom ${SOURCES})
target_link_libraries(GeneticKingdom sfml-graphics sfml-window sfml-system)
