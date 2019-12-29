cmake_minimum_required(VERSION 3.2)

message("***$ENV TRAVIS_BUILD_NUMBER}***")
message("$ENV{TRAVIS_BUILD_NUMBER}")
message("*******")
if(TRAVIS_BUILD_NUMBER)
    project(cppcicd VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(cppcicd VERSION 0.0.1)
endif()
#project(cppcicd VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})


find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(MSVC)
    find_package(Threads REQUIRED)
endif()

find_package(GTest REQUIRED)

configure_file(version.h.in version.h)

add_executable(cppcicd_cli main.cpp)
add_library(cppcicd lib.cpp)

add_executable(test_version test_version.cpp)

add_executable(g_test_version
        g_test_version.cpp
        lib.cpp
        lib.h
)

set_target_properties(cppcicd_cli cppcicd test_version g_test_version PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON)

target_include_directories(cppcicd PRIVATE "${CMAKE_BINARY_DIR}")

target_include_directories(g_test_version PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${GTEST_INCLUDE_DIRS}"
        )

set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
        )

target_link_libraries(cppcicd_cli PRIVATE
        cppcicd
        )

target_link_libraries(test_version
        ${Boost_LIBRARIES}
        cppcicd
        )

include_directories(${GTEST_INCLUDE_DIR})
if(MSVC)
    target_link_libraries(g_test_version ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)
else ()
    target_link_libraries(g_test_version ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
endif()

if(MSVC)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(cppcicd_cli PRIVATE
            /W4
            )
    target_compile_options(cppcicd PRIVATE
            /W4
            )
    target_compile_options(test_version PRIVATE
            /W4
            )
    target_compile_options(g_test_version PRIVATE
            /W4
            )
else ()
    target_compile_options(cppcicd_cli PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    target_compile_options(cppcicd PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
    target_compile_options(g_test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
            )
endif()


install(TARGETS cppcicd_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
add_test(g_test_version g_test_version)


