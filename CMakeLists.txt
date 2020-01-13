cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME  MyMapGui)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENABLE_TESTS OFF)

IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    MESSAGE(STATUS "current platform: Linux ")
    set(CMAKE_PREFIX_PATH "/home/lix/program/Qt/5.12.5/gcc_64/lib/cmake/")
    find_package(Boost COMPONENTS signals REQUIRED)
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    MESSAGE(STATUS "current platform: Windows")
    set(BOOST_ROOT "D:\\cpp_libs\\boost_1_70_0_build")
    find_package(Boost REQUIRED)
    include_directories(${Boost_INCLUDE_DIR})
    set(CMAKE_PREFIX_PATH "D:\\QT\\5.12.6\\msvc2017_64\\lib\\cmake")
ENDIF (CMAKE_SYSTEM_NAME MATCHES "Linux")


find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(include)

file(GLOB QT_HEADERS "./include/gui/*.h")
foreach(file ${QT_HEADERS})
    message(STATUS Gui Files : ${file})
endforeach()
qt5_wrap_cpp(QT_MOCS ${QT_HEADERS})

file(GLOB_RECURSE SRC "./src/*.cpp")

find_package (Threads REQUIRED)

add_executable(${PROJECT_NAME}
        ${SRC}
        ${QT_MOCS}
        ${Boost_LIBRARIES}
        main.cpp
        ./src/gui/mainwindow.ui
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES})

if(${ENABLE_TESTS})
    MESSAGE(STATUS "ENABLE TESTS IS ON")
    add_subdirectory(tests)
endif()
