PROJECT(goblin-engineer-example-http)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        context.cpp
        context.hpp
        http_connection.cpp
        http_connection.hpp
        dto.cpp
        dto.hpp
        network_service.cpp
        network_service.hpp
)

target_link_libraries (
        ${PROJECT_NAME} PRIVATE
        goblin-engineer
        ${CONAN_LIBS}
)