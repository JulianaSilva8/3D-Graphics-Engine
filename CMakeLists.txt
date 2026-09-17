cmake_minimum_required(VERSION 3.5)

project(CG2526)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# generator
file(GLOB GENERATOR_PRIMITIVES
    CONFIGURE_DEPENDS
    src/generator/graphical_primitives/*.cpp
    src/generator/graphical_primitives/*.cp
    src/generator/patches.cpp
)

add_executable(generator
    src/generator/main.cpp
    ${GENERATOR_PRIMITIVES}
)

target_include_directories(generator PRIVATE include)

# engine
add_executable(engine
    src/engine/main.cpp
    src/engine/Engine.cpp
    src/engine/Parser.cpp
    src/engine/Scene.cpp
    src/engine/Window.cpp
    src/engine/Camera.cpp
    src/engine/Group.cpp 
    src/engine/Model.cpp
    src/engine/Light.cpp
    src/engine/transformations/StaticTransformation.cpp
    src/engine/transformations/CurveTransformation.cpp
    src/engine/transformations/RotationTimeTransformation.cpp
    src/utils/utils.cpp 
    src/utils/CatmullRomCurve.cpp
)

target_include_directories(engine PRIVATE include)

find_package(OpenGL REQUIRED)

if (WIN32)
    message(STATUS "Toolkits_DIR set to: ${TOOLKITS_FOLDER}")
    set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")

    if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
        message(FATAL_ERROR "GLUT not found in TOOLKITS_FOLDER")
    endif()

    target_include_directories(engine PRIVATE ${TOOLKITS_FOLDER}/glut)
    target_link_libraries(engine PRIVATE ${OPENGL_LIBRARIES} ${TOOLKITS_FOLDER}/glut/glut32.lib)

    if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll")
        file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
    endif()
else()
    find_package(GLUT REQUIRED)
    find_package(GLEW REQUIRED)

    if (NOT GLUT_FOUND)
        message(FATAL_ERROR "GLUT not found")
    endif()

    target_include_directories(engine PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
    target_link_libraries(engine PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} GLEW::GLEW)
endif()