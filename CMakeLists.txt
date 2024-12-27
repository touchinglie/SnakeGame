cmake_minimum_required(VERSION 3.16)
project(SnakeGame C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(GTK4_INCLUDE_DIRS "E:/msys64/mingw64/include/")
set(GTK4_LIBRARY_DIRS "E:/msys64/mingw64/lib/")
set(GSTREAMER_INCLUDE_DIRS "E:/msys64/mingw64/include/")
set(GSTREAMER_LIBRARY_DIRS "E:/msys64/mingw64/lib/")
# set(PTHREAD_INCLUDE_DIRS "G:/programing/c/pthread/include")
set(ENV{PKG_CONFIG_PATH} "E:/msys64/mingw64/lib/")

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

include_directories(${GTK4_INCLUDE_DIRS})
link_directories(${GTK4_LIBRARY_DIRS})
add_definitions(${GTK4_CFLAGS_OTHER})
include_directories(${GSTREAMER_INCLUDE_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})
add_definitions(${GSTREAMER_CFLAGS_OTHER})
# include_directories(${PTHREAD_INCLUDE_DIRS})

add_executable(SnakeGame snakeGameWin.c ${CMAKE_CURRENT_BINARY_DIR}/resources.c)
# set_target_properties(SnakeGame PROPERTIES WIN32_EXECUTABLE true) # this line makes the exe running without console 
target_link_libraries(SnakeGame ${GTK4_LIBRARIES})
target_link_libraries(SnakeGame ${GSTREAMER_LIBRARIES})
target_link_libraries(SnakeGame winmm.lib)
# target_link_libraries(SnakeGame libpthread-static.lib)

set(GLIB_FLAGS --target=resources.c)
set(GLIB_FLAGS ${GLIB_FLAGS} --generate-source)
set(GLIB_FLAGS ${GLIB_FLAGS} --sourcedir=${PROJECT_SOURCE_DIR})

add_custom_command(
        OUTPUT resources.c
        COMMAND glib-compile-resources ${PROJECT_SOURCE_DIR}/snakeGame.gresource.xml ${GLIB_FLAGS}
        DEPENDS snakeGame.gresource.xml
        res/micon.ico
        res/skaterswaltz_8bit.mp3
        res/Rockman3Shortend.mp3
        res/ate_8bit.mp3
)

add_custom_target(generate_resources DEPENDS resources.c)
add_dependencies(${PROJECT_NAME} generate_resources)