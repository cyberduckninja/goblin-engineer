cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)

set(project goblin-engineer)

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

#############################################################################################
#BOOST

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.13/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_run(
            CONANFILE conanfile.txt
            BASIC_SETUP
            BUILD missing
            NO_OUTPUT_DIRS
    )

include_directories (SYSTEM ${Boost_INCLUDE_DIRS})

#############################################################################################

list(APPEND ${PROJECT_NAME}_header
        header/goblin-engineer/detail/converters.hpp
        header/goblin-engineer/detail/constructors.hpp
        header/goblin-engineer/detail/object.hpp
        header/goblin-engineer/abstract_service.hpp
        header/goblin-engineer/context.hpp
        header/goblin-engineer/dynamic.hpp
        header/goblin-engineer/dynamic_environment.hpp
        header/goblin-engineer/forward.hpp
)

list(APPEND ${PROJECT_NAME}_source
        source/abstract_service.cpp
        source/dynamic.cpp
        source/dynamic_environment.cpp
)

add_library(
        ${PROJECT_NAME}
        ${${PROJECT_NAME}_header}
        ${${PROJECT_NAME}_source}
)

target_link_libraries (
        ${PROJECT_NAME} PUBLIC
        actor-zeta
        ${CONAN_LIBS}
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
)

add_subdirectory(example)