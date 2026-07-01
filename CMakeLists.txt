cmake_minimum_required(VERSION 3.23)

#Gets SFML 3.1 From GitHub
include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)

FetchContent_MakeAvailable(SFML)

#Project Name
project("Pongdemonium")
#Set C++ Version
set(CMAKE_CXX_STANDARD 17)

#Code To Add All Source Files To Variable
#file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pong/src/*.cpp")
#Adds All Files In "/src" To "MY_SOURCES" Variable
set (MY_SOURCES
        "pong/src/Ball.cpp"
        "pong/src/Button.cpp"
        "pong/src/Chaos.cpp"
        "pong/src/GameEngine.cpp"
        "pong/src/main.cpp"
        "pong/src/Paddle.cpp"
        "pong/src/Timer.cpp"
)

#Adds The Sources To The Application
add_executable("${CMAKE_PROJECT_NAME}" "${MY_SOURCES}")

#Copies All Folders And Files In "pong/assets" To Build Folder For Application To Access
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/pong/assets
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets
)

#Adds SFML Library To Project
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
)

#Makes SFML Static
set(SFML_STATIC_LIBRARIES TRUE)

#Adds The Header Files To The Project
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pong/include")
