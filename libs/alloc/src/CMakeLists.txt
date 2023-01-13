set( LIB_NAME llc-alloc )

add_library( ${LIB_NAME} STATIC
    Align.cpp
    Allocator.cpp
    Block.cpp
    Header.cpp
)


target_include_directories( ${LIB_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries( ${LIB_NAME}
    PUBLIC
)

