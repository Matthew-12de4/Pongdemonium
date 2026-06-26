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
#Adds All Files In "/src" To "MY_SOURCES" Variable
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/pong/src/*.cpp")

#Adds The Sources To The Application
add_executable("${CMAKE_PROJECT_NAME}" "${MY_SOURCES}")

#Copies All Folders And Files In "/Assets" To Build Folder For Application To Access
add_custom_command(TARGET pong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/pong/assets
        $<TARGET_FILE_DIR:pong>/assets
)

#Adds SFML Library To Project
target_link_libraries(pong PRIVATE
    SFML::System
    SFML::Window
    SFML::Graphics
    SFML::Audio
)

#Makes SFML Static
set(SFML_STATIC_LIBRARIES TRUE)

#Adds The Header Files To The Project
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/pong/include")
