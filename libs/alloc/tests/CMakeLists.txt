set( TEST_NAME alloc-utests )

add_executable( ${TEST_NAME}
    Allocator.cpp
    main.cpp
)

target_link_libraries( ${TEST_NAME}
    PUBLIC
    lcc-alloc
    gtest
    spdlog
)
