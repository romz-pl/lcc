set( TEST_NAME string-utests )

add_executable( ${TEST_NAME}
    StringPool.cpp
    main.cpp
)

target_link_libraries( ${TEST_NAME}
    PUBLIC
    lcc-string
    gtest
    spdlog
)
