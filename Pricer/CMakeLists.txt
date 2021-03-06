cmake_minimum_required(VERSION 2.8)
project(mc-pricer CXX)

# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()


# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Add math lib
set(LIBS ${LIBS} m)

# Testing the parser
#add_executable(parser-test src/parser.cpp Tests/test_parser.cpp)
#target_link_libraries(parser-test ${LIBS})


#add_executable(mc-example Tests/mc_example.cpp)
#target_link_libraries(mc-example ${LIBS})
 
# add_executable(mc-pricer src/parser.cpp src/pricer.cpp)
# target_link_libraries(mc-pricer ${LIBS})
# install(TARGETS mc-pricer DESTINATION bin)

add_subdirectory(src)
add_subdirectory(Tests)
#Configuration de l'exécutable

file(

        GLOB_RECURSE

        source_files
      
        src/*

)

add_executable(

        pricer
        
        #Tests/test_parser.cpp
        ${source_files}

)
#target_link_libraries(pricer Cpp)
target_link_libraries(pricer ${LIBS})
#install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/pricer}
 #   DESTINATION bin
  #  RENAME ${CMAKE_PROJECT_NAME}-pricer)
#add_test(TestName ExeName)
#add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
 #                 DEPENDS ExeName)

#set(CMAKE_CXX_FLAGS "-c")

# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
