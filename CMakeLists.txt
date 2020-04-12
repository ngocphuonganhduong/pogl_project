cmake_minimum_required(VERSION 3.15)
project(pogl)
set(CMAKE_CXX_STANDARD 17)

set(HEADER
            src/matrix4.hh
            src/object.hh)

set(SOURCE src/image.cpp
            src/image_io.cpp
            src/matrix4.cpp
            src/object.cpp
            src/program.cpp
            src/init.cpp
        )
add_executable(pogl src/main.cpp ${HEADER} ${SOURCE})
add_executable(test1 src/test1.cpp ${HEADER} ${SOURCE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)


target_link_libraries(pogl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} GLEW::GLEW)
target_link_libraries(test1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} GLEW::GLEW)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
