
cmake_minimum_required(VERSION 3.0)

project(cmakeclient)

include_directories("src/include")

set (SRC_LIST src/main.c
			  src/controller.c
			  src/net/socket.c
			  src/net/net.c
			  src/math/vecmath.c
			  src/world.c
			  src/graphics/objects/pp.c
			  src/graphics/objects/box.c
			  src/graphics/objects/field.c
			  src/graphics/objects/sphere_sector.c
			  src/graphics/objects/rect.c
			  src/graphics/objects/circle.c
			  src/graphics/objects/sphere.c
			  src/graphics/glfw.c
			  src/graphics/ft.c
			  src/graphics/material.c
			  src/graphics/shader.c
			  src/graphics/draw_obj.c
			  src/graphics/gl/pp.c
			  src/graphics/gl/depth.c
			  src/graphics/gl/light.c
			  src/graphics/gl/bars.c
			  src/graphics/gl.c
			  src/util/timing.c
			  src/util/file.c
			  src/util/log.c
			  )
			  
set(CMAKE_CXX_FLAGS "-std=c++11 -march=native -pipe -Wall -Wextra -Wshadow -I/usr/include/freetype2 -Bstatic -Bdynamic -ldl -lpthread")
set(CMAKE_C_FLAGS "-std=c99 -march=native -pipe -Wall -Wextra -Wshadow -I/usr/include/freetype2 -Bstatic  -Bdynamic")

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} PUBLIC m GL GLEW glfw freetype X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor)
