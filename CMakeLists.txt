cmake_minimum_required(VERSION 3.18) 
project(ionbeta LANGUAGES CXX)

find_package(ROOT REQUIRED COMPONENTS
             Core RIO Net Hist Graf Gpad Tree Rint)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  

file(GLOB_RECURSE SRC_FILES       CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.c*")
file(GLOB_RECURSE PUBLIC_HEADERS  CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/include/*.h*")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${PUBLIC_HEADERS})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include          
        ${ROOT_INCLUDE_DIRS})                

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${ROOT_LIBRARIES})                   

if(EXISTS "${ROOT_USE_FILE}")
    include(${ROOT_USE_FILE})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

message(STATUS "Source files (${PROJECT_NAME}):")
foreach(f IN LISTS SRC_FILES)  
    message(STATUS "  ${f}")
endforeach()
