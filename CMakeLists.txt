cmake_minimum_required(VERSION 3.12)
project(RLogSU LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

###=========================================== DEPENDENCIES ==========================================
include(FetchContent)

find_package(fmt QUIET)

if(NOT fmt_FOUND)
    message(STATUS "fmt not found in system, fetching via FetchContent...")
    
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG        8.1.1
    )
    
    FetchContent_MakeAvailable(fmt)
else()
    message(STATUS "fmt found in system, using system version.")
endif()
###=========================================== /DEPENDENCIES =========================================

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger_impl.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    fmt::fmt
)