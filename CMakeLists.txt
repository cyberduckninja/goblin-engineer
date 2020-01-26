cmake_minimum_required(VERSION 3.0)

set (CMAKE_PROJECT_VERSION_MAJOR 1)
set (CMAKE_PROJECT_VERSION_MINOR 0)
set (CMAKE_PROJECT_VERSION_PATCH 0)
set (CMAKE_PROJECT_VERSION "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}")

cmake_policy(SET CMP0048 NEW)
PROJECT(goblin-engineer VERSION "${CMAKE_PROJECT_VERSION}" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ALLOW_EXAMPLES "Enable building examples" OFF)

option(DEV_MODE "developer mode" OFF)

if (DEV_MODE)
    add_compile_options(
            -v
            #-Werror

            -Wall
            -Wextra
            -Wpedantic

            -Wcast-align
            -Wcast-qual
            #-Wconversion #-> boost problem
            -Wctor-dtor-privacy
            -Wenum-compare
            #-Wfloat-equal
            #-Wnon-virtual-dtor #->asio problem
            #-Wold-style-cast #-> boost problem
            -Woverloaded-virtual
            -Wredundant-decls
            #-Wsign-conversion #-> boost problem
            -Wsign-promo
    )
endif ()

#############################################################################################
#CONAN

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/v0.14/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
    endif()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    #conan_add_remote(NAME jinntechio INDEX 1 URL https://api.bintray.com/conan/jinntechio/public-conan)

    conan_cmake_run(
            CONANFILE conanfile.txt
            BASIC_SETUP
            BUILD missing
            NO_OUTPUT_DIRS
    )

include_directories (SYSTEM ${Boost_INCLUDE_DIRS})
#CONAN
#############################################################################################

list(APPEND ${PROJECT_NAME}_header

        header/goblin-engineer/components/network.hpp

        header/goblin-engineer/detail/converters.hpp
        header/goblin-engineer/detail/constructors.hpp
        header/goblin-engineer/detail/object.hpp

        header/goblin-engineer/abstract_manager_service.hpp
        header/goblin-engineer/abstract_service.hpp
        header/goblin-engineer/dynamic.hpp
        header/goblin-engineer/root_manager.hpp
        header/goblin-engineer/forward.hpp
)

list(APPEND ${PROJECT_NAME}_source
        source/abstract_manager_service.cpp
        source/dynamic.cpp
        source/root_manager.cpp
        source/network.cpp
)

add_library(
        ${PROJECT_NAME}
        ${${PROJECT_NAME}_header}
        ${${PROJECT_NAME}_source}
)

target_link_libraries (
        ${PROJECT_NAME} PRIVATE
        ${CONAN_LIBS}
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/header
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/header/goblin-engineer
)

if(ALLOW_EXAMPLES)
    add_subdirectory(examples)
endif()