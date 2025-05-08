cmake_minimum_required(VERSION 3.15)
project(GeneticKingdom)

set(CMAKE_CXX_STANDARD 17)

# Permitir elegir tipo de build (Debug/Release) desde la línea de comandos:
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Choose the type of build, options are: Debug Release" FORCE)
endif()

# Opciones de compilación comunes
add_definitions(-DSFML_STATIC_LIBRARIES=FALSE)

# Si estamos en modo Debug, activamos símbolos y sanitizers
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS ">>> Enabling Debug flags and AddressSanitizer")
  add_compile_options(-g -O1 -fsanitize=address,undefined)
  add_link_options   (-fsanitize=address,undefined)
endif()

# Encontrar SFML con audio
find_package(SFML 2.5 COMPONENTS system window graphics audio REQUIRED)

# Directorios de headers
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/include
)

# Recopilar todos los .cpp
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(GeneticKingdom ${SOURCES})

# Enlazar SFML
target_link_libraries(GeneticKingdom
    PRIVATE
        sfml-system
        sfml-window
        sfml-graphics
        sfml-audio
)

# Copiar assets tras la compilación
add_custom_command(TARGET GeneticKingdom POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_BINARY_DIR}/assets
)
