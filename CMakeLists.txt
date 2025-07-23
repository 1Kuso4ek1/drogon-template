cmake_minimum_required(VERSION 3.10)
project(backend)

# For drogon
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTING OFF)

# For JWT
set(JWT_BUILD_EXAMPLES OFF)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

set(
    BACKEND_HEADERS

    include/Controllers/LoginController.hpp
    include/Controllers/StaticController.hpp

    include/Filters/JwtFilter.hpp

    include/Utils/JwtUtils.hpp
    include/Utils/Singleton.hpp
)

set(
    BACKEND_SOURCES

    src/Controllers/LoginController.cpp
    src/Controllers/StaticController.cpp

    src/Filters/JwtFilter.cpp

    src/main.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libraries/drogon)
add_subdirectory(libraries/jwt-cpp)

add_executable(
    backend

    ${BACKEND_HEADERS}
    ${BACKEND_SOURCES}
)


target_link_libraries(backend drogon jwt-cpp)

target_include_directories(backend PUBLIC ${CMAKE_SOURCE_DIR}/include/)

