cmake_minimum_required(VERSION 3.10)
project(SFMLGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 3.0 COMPONENTS System Window Graphics Audio REQUIRED)

# Add executable
add_executable(${PROJECT_NAME} Character.cpp)

# Link SFML libraries
target_link_libraries(${PROJECT_NAME} SFML::System SFML::Window SFML::Graphics SFML::Audio)
target_include_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/include) 