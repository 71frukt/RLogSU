cmake_minimum_required(VERSION 3.12)
project(RLogSU VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(USE_LOGGING "Enable logging support" ON)


#--- GET LOG DIR --------------------------------------------------------
get_property(parent_log_dir DIRECTORY .. PROPERTY LOG_DIR)

if(parent_log_dir)
    set(LOG_DIR ${parent_log_dir})
    message(STATUS "Using parent LOG_DIR: ${LOG_DIR}")
    set_property(DIRECTORY PROPERTY LOG_DIR ${parent_log_dir})
else()
    set(LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log)
    message(STATUS "Using and creating default LOG_DIR: ${LOG_DIR}")
    file(MAKE_DIRECTORY ${LOG_DIR})
    set_property(DIRECTORY PROPERTY LOG_DIR "\"${CMAKE_CURRENT_SOURCE_DIR}/log\"")
endif()

#------------------------------------------------------------------------


add_library(${PROJECT_NAME} STATIC
    src/logger.cpp
    # main.cpp/
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(USE_LOGGING)
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_LOGGING)
    message(STATUS "Logging ENABLED")
else()
    message(STATUS "Logging DISABLED")
endif()

target_compile_definitions(RLogSU PRIVATE MODULE_NAME="${PROJECT_NAME}")

add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)