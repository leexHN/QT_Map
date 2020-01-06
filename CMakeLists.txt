cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME  MyMapGui)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/lix/program/Qt/5.12.5/gcc_64/lib/cmake/")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(include)

file(GLOB QT_HEADERS "./include/gui/*.h")
foreach(file ${QT_HEADERS})
    message(STATUS Gui Files : ${file})
endforeach()
qt5_wrap_cpp(QT_MOCS ${QT_HEADERS})

file(GLOB_RECURSE SRC "./src/*.cpp")

find_package (Threads)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${QT_MOCS}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets
        ${CMAKE_THREAD_LIBS_INIT})