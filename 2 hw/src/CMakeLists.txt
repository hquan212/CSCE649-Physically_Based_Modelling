find_package(OpenGL)
set(LIBS ${LIBS} ${OPENGL_LIBRARIES})

find_package(GLUT)
set(LIBS ${LIBS} ${GLUT_LIBRARIES})
include_directories(${GLUT_INCLUDE_DIR})

set(SRCS
    ball.cpp
    ballgen.cpp
    physicsgen.cpp
    
)

add_executable(particle 
    ${SRCS}
    main.cpp
)

set_target_properties(particle PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_link_libraries(particle ${LIBS})


add_library(particle_lib STATIC ${SRCS})

set_target_properties(particle_lib PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
target_link_libraries(particle_lib ${LIBS})