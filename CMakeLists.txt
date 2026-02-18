cmake_minimum_required(VERSION 3.20)
project(roblox-clone VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ROBLOX_CLONE_BUILD_TESTS "Build tests" ON)
option(ROBLOX_CLONE_BUILD_EDITOR "Build editor" ON)

find_package(spdlog CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(enet CONFIG REQUIRED)
find_package(ImGui CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

add_subdirectory(src)

if(ROBLOX_CLONE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
