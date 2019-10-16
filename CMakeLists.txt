cmake_minimum_required(VERSION 3.7)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -fopenmp -fsigned-char -DBOOST_NO_CXX11_SCOPED_ENUMS")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -gdwarf-3")
set(LIBDIR "/usr/lib" CACHE STRING "Some user-specified option")
set(INCLUDEDIR "/usr/include" CACHE STRING "Some user-specified option")
set(PYTHON_ONLY TRUE CACHE STRING "Some user-specified option")
message("CMakeInstall Prefix: ${CMAKE_INSTALL_PREFIX}")
############################################################################################################
project(nanopore-toolkit)
############################################################################################################
# GTEST INSTALL
############################################################################################################
if(APPLE)
    set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS} -undefined dynamic_lookup")
endif(APPLE)
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)
# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()
message(gtest_SOURCE_DIR: ${gtest_SOURCE_DIR})
############################################################################################################
# BOOST LIBRARY
############################################################################################################
#set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost 1.69.0 COMPONENTS system date_time filesystem iostreams coroutine context REQUIRED)
#find_package(hdf5 1.8 REQUIRED HINTS
#        ${CMAKE_INSTALL_PREFIX}/include
#        )
message(Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

############################################################################################################
# ZLIB LIBRARY
############################################################################################################
find_package(ZLIB)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()
############################################################################################################
# HDF LIBRARY
############################################################################################################
find_library(HDF5_LIBRARIES libhdf5.a
        HINTS
        ${CMAKE_INSTALL_PREFIX}/lib
        /usr/lib
        ${LIBDIR})

find_path(HDF5_INCLUDE_DIRS hdf5.h
        HINTS ${HDF5_LIBRARIES}
        ${CMAKE_INSTALL_PREFIX}/include
        ${INCLUDEDIR}
        /usr/include)
message(HDF5_INCLUDE_DIRS: "${HDF5_INCLUDE_DIRS}")
message(HDF5_LIBRARIES: "${HDF5_LIBRARIES}")
include_directories(${HDF5_INCLUDE_DIRS})
############################################################################################################
############################################################################################################
############################################################################################################
# my includes
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/fast5/include)
include_directories(${CMAKE_SOURCE_DIR}/fast5/include/fast5)
# my sources
set(MAIN_CPP src/main.cpp)
set(TEST_CPP tests/TestEventDetect.cpp tests/TestEventDetect.hpp)
set(C_SOURCE
        src/scrappie_common.c src/scrappie_common.h
        src/scrappie_stdlib.h
        src/scrappie_structures.h
        src/event_detection.c src/event_detection.h)
set(CPP_SOURCE
        src/EventDetection.cpp src/EventDetection.hpp
        src/EventDetectMain.cpp src/EventDetectMain.hpp
        src/Utils.cpp src/Utils.hpp)
set(SOURCE ${CPP_SOURCE} ${C_SOURCE} src/EventDetectMain.cpp src/EventDetectMain.hpp)
set(MY_LIBS ${HDF5_LIBRARIES} ${ZLIB_LIBRARIES} ${Boost_LIBRARIES})
############################################################################################################
# main
add_executable(nanopore-cli ${MAIN_CPP} ${SOURCE})
target_link_libraries(nanopore-cli ${MY_LIBS})
# pybind
add_subdirectory(submodules/pybind11)
pybind11_add_module(bindings src/event_detect/PythonBindings.cpp ${SOURCE})
target_link_libraries(bindings PRIVATE ${MY_LIBS})

############################################################################################################

# testing
add_executable(test_event_detect_cpp ${TEST_CPP} ${SOURCE})
target_link_libraries(test_event_detect_cpp ${MY_LIBS} gtest_main gmock_main)
enable_testing()
add_test(tests test_event_detect_cpp ${CMAKE_SOURCE_DIR})

# python tests
message(PYTHON_EXECUTABLE: "${PYTHON_EXECUTABLE}")
add_test(NAME test_event_detect COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tests/test_event_detect.py
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# make check command for testing
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
