cmake_minimum_required(VERSION 3.15)
project(pogl)
set(CMAKE_CXX_STANDARD 17)

set(HEADER src/stb_image.h
        src/utils/misc.hh
        src/utils/matrix4.hh
        src/utils/vector3.hh
        src/vbo/teapot.hh
        src/vbo/rectangle.hh
        src/opengl_handling_error.hh
        src/opengl_object.hh
        src/texture.hh
        src/program.hh
        src/init.hh
        src/obj_loader.hh
            src/frame_buffer.hh
        )

set(SOURCE
        src/utils/misc.cpp
        src/utils/matrix4.cpp
        src/opengl_handling_error.cpp
        src/program_object.cpp
        src/opengl_object.cpp
        src/program.cpp
        src/init.cpp
        src/texture.cpp
        src/obj_loader.cpp
            src/frame_buffer.cpp
        )

add_executable(pogl src/main.cpp ${HEADER} ${SOURCE})

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)


target_link_libraries(pogl ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} GLEW::GLEW)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS})
