cmake_minimum_required(VERSION 3.0)

cmake_policy(SET CMP0048 NEW)
PROJECT(goblin-engineer-example-http)

add_executable(
        ${PROJECT_NAME}
        context.cpp
        context.hpp
        http_connection.cpp
        http_connection.hpp
        dto.cpp
        dto.hpp
        main.cpp
        network_service.cpp
        network_service.hpp
)

target_link_libraries (
        ${PROJECT_NAME} PRIVATE
        goblin-engineer
)