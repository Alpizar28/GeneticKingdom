cmake_minimum_required(VERSION 3.15)
project(GeneticKingdom)

set(CMAKE_CXX_STANDARD 17)
set(SFML_STATIC_LIBRARIES FALSE)

# 1) Buscamos SFML con Audio
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# 2) Directorios de headers (para que incluya Constantes.h, Map.h, etc.)
#    Asumiendo que Constantes.h está en la raíz y el resto en src/
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# 3) Listado completo de fuentes
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(GeneticKingdom ${SOURCES})

# 4) Enlazamos los módulos de SFML
target_link_libraries(GeneticKingdom
    PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

add_custom_command(TARGET GeneticKingdom POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)

