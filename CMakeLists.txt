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
#ACTOR-ZETA

option(STANDALONE-ACTOR-ZETA ""  OFF)

if(STANDALONE-ACTOR-ZETA)
    set(path_actor_zeta ${actor-zeta-dir})
else()
    add_subdirectory(dependency/actor-zeta)
    set(path_actor_zeta "dependency/actor-zeta")
endif()

include_directories(SYSTEM ${path_actor_zeta}/core)
include_directories(SYSTEM ${path_actor_zeta}/io)

#ACTOR-ZETA
#############################################################################################

#############################################################################################
#BOOST

option(CONAN ""  OFF)

if(CONAN)

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

else()

    set(BOOST_COMPONENTS
            thread
            date_time
            system
            filesystem
            timer
            chrono
            program_options
            unit_test_framework
            locale
            )

    find_package (Boost 1.66 REQUIRED COMPONENTS ${BOOST_COMPONENTS})

endif()

include_directories (SYSTEM ${Boost_INCLUDE_DIRS})

#############################################################################################

include_directories(header)

list(APPEND ${PROJECT_NAME}_header

        header/goblin-engineer/abstract_plugin.hpp
        header/goblin-engineer/abstract_service.hpp
        header/goblin-engineer/configuration.hpp
        header/goblin-engineer/constructors.hpp
        header/goblin-engineer/context.hpp
        header/goblin-engineer/converters.hpp
        header/goblin-engineer/data_provider.hpp
        header/goblin-engineer/dynamic.hpp
        header/goblin-engineer/dynamic_environment.hpp
        header/goblin-engineer/forward.hpp
        header/goblin-engineer/metadata.hpp
        header/goblin-engineer/object.hpp
        header/goblin-engineer/plugin.hpp
        )

list(APPEND ${PROJECT_NAME}_source
        source/data_provider.cpp
        source/dynamic.cpp
        source/dynamic_environment.cpp
        source/plugin.cpp
)

add_library(
        ${PROJECT_NAME}
        ${${PROJECT_NAME}_header}
        ${${PROJECT_NAME}_source}
)

target_link_libraries (
        ${PROJECT_NAME} PUBLIC
        ${Boost_LIBRARIES}
        actor-zeta
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
)