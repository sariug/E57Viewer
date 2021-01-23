cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)

project(E57Viewer VERSION 1.0)
set(E57Viewer 1.0)

FUNCTION(PREPEND var )
   SET(listVar "")
   FOREACH(f ${${var}})
       LIST(APPEND listVar "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)


# Find GLFW
find_package(glfw3 3.3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_directories(${GLFW_LIBRARY_DIRS})
add_definitions(${GLFW_DEFINITIONS})

# Find GLEW 
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_directories(${GLEW_LIBRARY_DIRS})
add_definitions(${GLEW_DEFINITIONS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OpenGL_FOUND)
        message(Error "OpenGL not found")
endif(NOT OpenGL_FOUND)


add_subdirectory( extern/imgui )
add_subdirectory( extern/libE57Format )
add_subdirectory( include )
add_subdirectory( src )



add_executable(e57viewer  ${E57Viewer_SRC} ${E57Viewer_INC} )


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")
message(${E57Viewer_SOURCE_DIR}/extern/*)
target_include_directories(e57viewer PUBLIC
   # headers  to include when building from source
   ${E57Viewer_SOURCE_DIR}/extern/imgui
 #  ${E57Viewer_BINARY_DIR}/include
   )

target_link_libraries(e57viewer imgui E57Format glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

# Install Stuff