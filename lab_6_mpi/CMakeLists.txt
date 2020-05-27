cmake_minimum_required(VERSION 3.15)
project(mpi_heat_transfer CXX)

set(CMAKE_CXX_STANDARD 17)

## PVS Studio
#include(PVS-Studio.cmake)
#SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

## include Threads
#find_package(Threads REQUIRED)

## include Boost
find_package(Boost COMPONENTS filesystem program_options system locale REQUIRED)
set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
#set(Boost_USE_MULTITHREAD ON)  # enable maltreating
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

## include MPI
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

## include Headers
INCLUDE_DIRECTORIES(includes)

file(GLOB_RECURSE SRC "includes/*.h" "src/*.cpp")

add_executable(mpi_heat_transfer main.cpp ${SRC})
TARGET_LINK_LIBRARIES(mpi_heat_transfer LINK_PUBLIC ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(mpi_heat_transfer LINK_PUBLIC ${MPI_LIBRARIES})
#TARGET_LINK_LIBRARIES(mpi_heat_transfer LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT})

#####################################
## create own target for library
#pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
#        ANALYZE mpi_heat_transfer
#        OUTPUT FORMAT errorfile
#        MODE GA:1,2,3 OP:1,2,3 GA:1,2,3 OP:1,2,3 64:1,2,3 CS:1,2,3 MISRA:1,2,3
#        LOG target_${PROJECT_NAME}.err)
#####################################
