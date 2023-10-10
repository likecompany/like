cmake_minimum_required(VERSION 3.9)
project(like CXX)

set(CMAKE_CXX_STANDARD 23)

include(cmake/Packages.cmake)

add_executable(${PROJECT_NAME} src/main.cpp)

find_library(HIREDIS_LIB hiredis)
find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(${PROJECT_NAME} PRIVATE restinio::restinio glaze::glaze engine ${HIREDIS_LIB} ${REDIS_PLUS_PLUS_LIB})

find_path(HIREDIS_HEADER hiredis)
find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${PROJECT_NAME} PUBLIC ${HIREDIS_HEADER} ${REDIS_PLUS_PLUS_HEADER})
