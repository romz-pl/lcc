set( LIB_NAME lcc-alloc )

add_library( ${LIB_NAME} STATIC
    Alignment.cpp
    Allocator.cpp
    AllocatorBase.cpp
    Arena.cpp
    Block.cpp
    Global_Arena.cpp
    MathExtras.cpp
    MemAlloc.cpp
    SmallVector.cpp
    error_handler.cpp
    type_traits.cpp
)


target_include_directories( ${LIB_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries( ${LIB_NAME}
    PUBLIC
    spdlog
)

