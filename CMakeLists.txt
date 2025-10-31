cmake_minimum_required(VERSION 3.12)
project(RLogSU LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # to generate compile_commands.json

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#--- GET LOG DIR --------------------------------------------------------
if(NOT DEFINED LOG_DIR)
    set(LOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/log)
    file(MAKE_DIRECTORY ${LOG_DIR})
else()
endif()
#------------------------------------------------------------------------

find_package(fmt REQUIRED)


add_library(${PROJECT_NAME} STATIC
    src/logger.cpp
    src/graph.cpp
    # main.cpp/
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    fmt::fmt
)

# add_custom_target(run
#     COMMAND ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )


# if(NDEBUG)
#     target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
#     message(STATUS "${PROJECT_NAME}: NDEBUG ENABLED")
# else()
#     message(STATUS "${PROJECT_NAME}: NDEBUG DISABLED")
# endif()