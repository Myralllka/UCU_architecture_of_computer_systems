cmake_minimum_required(VERSION 3.15)
project(count_number_of_all_words)

set(CMAKE_CXX_STANDARD 17)

# PVS Studio
include(PVS-Studio.cmake)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()


set(CMAKE_BUILD_TYPE Debug)
find_package(Threads REQUIRED)
find_package(LibArchive 3.0.0)

set(Boost_USE_STATIC_LIBS OFF) # enable dynamic linking
set(Boost_USE_MULTITHREAD ON)  # enable maltreating
find_package(Boost COMPONENTS filesystem program_options system locale REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

file(GLOB_RECURSE SRC "includes/*.h" "src/*.cpp")
add_executable(count_number_of_all_words main.cpp ${SRC})
TARGET_LINK_LIBRARIES(count_number_of_all_words LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(count_number_of_all_words LINK_PUBLIC ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(count_number_of_all_words LINK_PUBLIC ${LibArchive_LIBRARIES})

add_executable(test test.cpp ${SRC})
TARGET_LINK_LIBRARIES(test LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(test LINK_PUBLIC ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(test LINK_PUBLIC ${LibArchive_LIBRARIES})

#####################################
## create own target for library
pvs_studio_add_target(TARGET ${PROJECT_NAME}_PVS.analyze ALL
        ANALYZE count_number_of_all_words
        OUTPUT FORMAT errorfile
        MODE GA:1,2,3 OP:1,2,3 GA:1,2,3 OP:1,2,3 64:1,2,3 CS:1,2,3 MISRA:1,2,3
        LOG target_${PROJECT_NAME}.err)

#####################################
