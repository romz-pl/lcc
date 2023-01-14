set( LIB_NAME lcc-alloc )

add_library( ${LIB_NAME} STATIC
    Arena.cpp
    Block.cpp
    Global_Arena.cpp
)


target_include_directories( ${LIB_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries( ${LIB_NAME}
    PUBLIC
    spdlog
)

