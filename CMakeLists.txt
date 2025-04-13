cmake_minimum_required(VERSION 3.10)
project(GeneticKingdom)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(GeneticKingdom ${SOURCES})
target_link_libraries(GeneticKingdom sfml-graphics sfml-window sfml-system)
