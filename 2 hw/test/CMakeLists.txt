find_package(GTest)

if(GTEST_FOUND)
    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(tests
        main.cpp
    )

    set_target_properties(tests PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

    set(LIBS particle_lib ${GTEST_LIBRARIES})
    target_link_libraries(tests ${LIBS})
endif()
