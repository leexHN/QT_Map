enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
include_directories(../include)

add_executable(TEST ${SRC} ${QT_MOCS} test_main.cpp)
target_link_libraries(TEST
        PRIVATE Qt5::Widgets
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})