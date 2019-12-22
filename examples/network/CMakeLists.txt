cmake_minimum_required(VERSION 3.0)

set(project goblin-engineer-example-network)

set (CMAKE_PROJECT_VERSION_MAJOR 1)
set (CMAKE_PROJECT_VERSION_MINOR 0)
set (CMAKE_PROJECT_VERSION_PATCH 0)
set (CMAKE_PROJECT_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")

cmake_policy(SET CMP0048 NEW)
PROJECT(${project} VERSION "${CMAKE_PROJECT_VERSION}" LANGUAGES CXX)

add_executable(
        ${PROJECT_NAME}
        main.cpp
)

target_link_libraries (
        ${PROJECT_NAME} PUBLIC
        goblin-engineer
        ${Boost_LIBRARIES}
        ${CONAN_LIBS}
)