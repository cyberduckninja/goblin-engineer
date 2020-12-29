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

option(CONAN_EXPERIMENTAL_INCLUDE "CONAN EXPERIMENT INCLUDE" ON)

option(HTTP_COMPONENT "developer mode" OFF)

if ((CMAKE_CXX_COMPILER_ID MATCHES "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
    add_compile_options(
            -Wall
            -Wextra
            -Wpedantic

            -Wcast-align
            -Wcast-qual
            -Wconversion
            -Wctor-dtor-privacy
            -Wenum-compare
            -Wfloat-equal
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Woverloaded-virtual
            -Wredundant-decls
            -Wsign-conversion
            -Wsign-promo
    )
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX)
endif ()

#############################################################################################
#CONAN

if (CONAN_EXPERIMENTAL_INCLUDE)
    set(CONAN_SYSTEM_INCLUDES "On")
endif ()

if (EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(NO_OUTPUT_DIRS)
else ()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif ()


include_directories (SYSTEM ${Boost_INCLUDE_DIRS})
#CONAN
#############################################################################################

set( ${PROJECT_NAME}_header_base
        header/goblin-engineer/components/root_manager.hpp
)

set( ${PROJECT_NAME}_source_base
        source/root_manager.cpp
)

set( ${PROJECT_NAME}_header_asio
        header/goblin-engineer/components/network.hpp
)

set( ${PROJECT_NAME}_source_asio
        source/network.cpp
)


set( ${PROJECT_NAME}_header_http
        header/goblin-engineer/components/http/http_server/http_session.hpp
        header/goblin-engineer/components/http/http_server/listener.hpp
        header/goblin-engineer/components/http/http_server/server.hpp
        header/goblin-engineer/components/http/http_server/websocket_session.hpp

        header/goblin-engineer/components/http/dispatcher/http_dispatcher.hpp
        header/goblin-engineer/components/http/dispatcher/router.hpp

        header/goblin-engineer/components/http/detail/network.hpp
        header/goblin-engineer/components/http.hpp
)

set( ${PROJECT_NAME}_source_http
        header/goblin-engineer/components/http/http_server/http_session.cpp
        header/goblin-engineer/components/http/http_server/listener.cpp
        header/goblin-engineer/components/http/http_server/server.cpp
        header/goblin-engineer/components/http/http_server/websocket_session.cpp
        header/goblin-engineer/components/http/dispatcher/http_dispatcher.cpp
)

set( ${PROJECT_NAME}_header
        header/goblin-engineer/abstract_manager_service.hpp
        header/goblin-engineer/abstract_service.hpp
        header/goblin-engineer/forward.hpp
)

set( ${PROJECT_NAME}_source
        source/abstract_manager_service.cpp
)


list(APPEND ${PROJECT_NAME}_header

        ${${PROJECT_NAME}_header_base}
        ${${PROJECT_NAME}_header_asio}
)

list(APPEND ${PROJECT_NAME}_source
        ${${PROJECT_NAME}_source_base}
        ${${PROJECT_NAME}_source_asio}
)

if(HTTP_COMPONENT)
    list(APPEND ${PROJECT_NAME}_header ${${PROJECT_NAME}_header_http})
    list(APPEND ${PROJECT_NAME}_source ${${PROJECT_NAME}_source_http})
endif()

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