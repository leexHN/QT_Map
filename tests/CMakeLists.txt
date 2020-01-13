enable_testing()

qt5_wrap_cpp(QT_MOCS_TEST ${QT_HEADERS})
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
include_directories(../include)

add_executable(TEST ${SRC} ${QT_MOCS_TEST} test_main.cpp)
target_link_libraries(TEST
        PRIVATE Qt5::Widgets
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${Boost_LIBRARIES})