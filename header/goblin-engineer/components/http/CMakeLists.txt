set(CURRENT_TARGET http)

list(APPEND ${CURRENT_TARGET}_HEADERS
        http_session.hpp
        http_server/server.hpp
        websocket_session.hpp
        forward.hpp
        router.hpp
        network.hpp
        router/http_dispather.hpp
)

list(APPEND ${CURRENT_TARGET}_SOURCES

)

add_library(rocketjoe_${CURRENT_TARGET} STATIC
        ${${CURRENT_TARGET}_HEADERS}
        ${${CURRENT_TARGET}_SOURCES}
)


add_library(rocketjoe::${CURRENT_TARGET} ALIAS rocketjoe_${CURRENT_TARGET})

set_property(TARGET rocketjoe_${CURRENT_TARGET} PROPERTY EXPORT_NAME ${CURRENT_TARGET})

target_link_libraries(
        rocketjoe_${CURRENT_TARGET} PRIVATE
        rocketjoe::network
        rocketjoe::http_server
        goblin-engineer
        PRIVATE pybind11::embed
)

target_include_directories(rocketjoe_${CURRENT_TARGET} PUBLIC "../../../../python_engine/header" "../../../../..")

