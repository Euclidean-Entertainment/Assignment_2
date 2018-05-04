# CMakeLists.txt from SILENT HILL 3 Redux by Palm Studios
# Copyright 2016-2018 Alexander Hirsch and Jesse Buhagiar

find_package(GLU REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

include_directories("../include")
include_directories(SYSTEM "${GLUT_INCLUDE_DIR}")
include_directories(SYSTEM "${OPENGL_INCLUDE_DIR}")

add_executable("${CMAKE_PROJECT_NAME}"

    "euclidean2/main.c"

	"euclidean2/system/engine.c"
	"euclidean2/system/window.c"

   # "euclidean2/math/vec2.c"
    "euclidean2/math/vec3.c"
)

target_link_libraries("${CMAKE_PROJECT_NAME}"

    PRIVATE "${OPENGL_LIBRARIES}"
    PRIVATE "${GLU_LIBRARIES}"
    PRIVATE "${GLUT_LIBRARIES}"
    PRIVATE "m"
)

