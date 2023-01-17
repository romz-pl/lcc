set( TEST_NAME alloc-utests )

add_executable( ${TEST_NAME}
    Alignment.cpp
    Allocator.cpp
    Arena.cpp
    SmallVector.cpp
    main.cpp
)

target_link_libraries( ${TEST_NAME}
    PUBLIC
    lcc-alloc
    gtest
    spdlog
)
