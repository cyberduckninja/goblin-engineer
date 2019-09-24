cmake_minimum_required(VERSION 2.8)

set(project goblin-engineer-example-base)

set (CMAKE_PROJECT_VERSION_MAJOR 1)
set (CMAKE_PROJECT_VERSION_MINOR 0)
set (CMAKE_PROJECT_VERSION_PATCH 0)
set (CMAKE_PROJECT_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")

if (CMAKE_VERSION VERSION_LESS 3.0)
    PROJECT(${project} CXX)
else()
    cmake_policy(SET CMP0048 NEW)
    PROJECT(${project} VERSION "${CMAKE_PROJECT_VERSION}" LANGUAGES CXX)
endif()

list(APPEND ${PROJECT_NAME}_header

)

list(APPEND ${PROJECT_NAME}_source
    main.cpp
)

add_executable(
        ${PROJECT_NAME}
        ${${PROJECT_NAME}_header}
        ${${PROJECT_NAME}_source}
)

target_link_libraries (
        ${PROJECT_NAME} PUBLIC
        goblin-engineer
        ${Boost_LIBRARIES}
        ${CONAN_LIBS}
)