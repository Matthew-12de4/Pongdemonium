# Matthew Jennings (P2914142) Pong Coursework

# Title: Pongdemonium

# Dependencies

- [Git](https://git-scm.com/)
- [Cmake](https://cmake.org/download/)
- Build Tools:
  - [Visual Studio](https://visualstudio.microsoft.com/downloads/?q=build+tools)
  - [Ninja](https://ninja-build.org/)
  - [GCC](https://gcc.gnu.org/install/index.html)

# How to run:
## Windows
- Execute the [buildWin.bat](#cmake-1)
  - This will compile, build and run the project

### Linux
- Execute the [buildLin.sh](#cmake-1)
  - This will compile, build and run the project

# Beyond The Spec:

## CMAKE

I changed the build tool from Visual Studio to CMake because I daily drive a linux machine meaning Visual Studio does not work on. I needed a one size fits all solution which was cmake I created the [CMakeLists.txt](#cmake-1) myself for my needs. It will automatically detect a complier it can use and build an executable file

## SFML Update:

I upgraded [SFML to 3.1.0](https://www.sfml-dev.org/documentation/3.1.0/?target=_blank) so I can use the most up-to-date tools and documentation, as well as to use my already existing knowledge of SMFL to my advantage and not re-learn it. I also had to upgrade C++ version from C++14 to C++17 which allows SFML 3.1.0 to be included.

### SFML 3.1 Changes

Some important and noteable changes to the original code are:

<details>
<summary>Before</summary>

``` cpp
GameEngine::GameEngine(sf::RenderWindow& window) 
    : m_window(window),
    m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),

m_font.loadFromFile(".\\assets\\fonts\\digital-7.ttf");

while (m_window.isOpen())
{
    dt = m_clock.restart().asSeconds();

    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) m_window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            m_window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            m_gStates = GameStates::playing;
    }
}
```

</details>

<details>
<summary>After</summary>

``` cpp
GameEngine::GameEngine(sf::RenderWindow& window)
    : m_window(window),
    m_hud(m_font), //Asked for defult constructor
    m_ballSound(m_ballBuffer), //Asked for defult constructor
    m_paddle1(sf::Vector2f(20, window.getSize().y / 2.f), 10, 100, sf::Color::White),

    if (!(m_font.openFromFile("./assets/fonts/digital-7.ttf")))
        return; //old function did not exist, replaced with new the function and a check

//Entire new event system
while (m_window.isOpen())
{
	dt = m_clock.restart().asSeconds();

	while (const std::optional event = m_window.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window.close();
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) { 
            //Checks if a key is pressed then checks what was pressed
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) 
            //scancodes are more efficient as they are prepared to handle different types of keyboards
				m_window.close();
			if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
				m_gStates = GameStates::playing;
		}
	}
}
```

</details>

<br>
I also had to change all vector values:

``` cpp
m_shape.setPosition( x, y );
//adding {} around 2 floats to create a vector value
m_shape.setPosition({ x, y });
```

## Options Menu
In the options menu there is volume control and a checkbox to enable/disable flashing lights (Disabled By Default). All the options persist via a JSON file

## Leaderboard
Each game mode has its own leaderboard to keep it fair. The leaderboard is made persistent through local JSON files. The leaderboard tracks: Player Score, Enemy Score, Time Alive, Chaos Effects Fought and Date

## Chaos Effects

### None
As the name suggests, there is no chaos effect

### Paddle Speed
Increases/Decreases the paddle speed

### Ball Speed
Increases/Decrease the ball speed

### Paddle Size
Increases/Decreases the paddle size

### Ball Size
Increases/Decreases the ball size

### Jesus Take The Wheel
This is a pun on the comedy phrase where an AI takes over your player character for the duration of the effect

### Jitter
While still bouncing around the ball will move around erratically along its path

### Move Anywhere
This allows you to move up and down as well as left and right

### Many Balls
This will spawn 1-50 extra balls

### Paddle Colour
Will flash several different colours for the duration of the effect

### Ball Colour
Will flash several different colours for the duration of the effect

### Invisible Paddle
Makes the paddle invisible

### Invisible Ball
Makes the ball invisible

### Rapid Background Colour
This will make the background flash several different colours. Can be disabled in options

### Fake Crash
Will momentarily pause the game

### Tripping
Will remove the clear command so any movement leaves a trail

### AI Aim Bot
Will intentionally make the AI impossible to win against

### Square Ball
Will make the ball a square

### Meta
Will play 3 different chaos effects at once

### Speedy Timer
Will decrease the timer duration to 2 seconds. the timer lasts at least 5 turns

### Cannot Win
Makes the balls bounce off the left and right wall

### Rick Ball
Turns the ball into the internet meme "Rick Roll" along with some music to fit

### Invert Controls
Will make all the controls backwards

### Switching Sides
Switch the player and the enemy around along with their scores (resets after duration)

### DVD Window
Physically moves the window around your desktop like the classic DVD logo

### Meow
This will make the game cat themed, changing all textures and sound effects to cat related ones

### Rotate Paddle
Makes the Paddle rotate 90 degrees

### Invert Colours
Inverts all the colours on the screen

### Laggy
Intentionally sets the FPS to 15 to create a lag effect

# CMAKE

This is all the files required to make CMake run easily

<details>
<summary>CMakeLists.txt</summary>

``` cmake
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
project("pong")
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
```

</details>
<details>
<summary>buildWin.bat</summary>
This batch file checks if the build folder exists, if it doesn't it will make it. It will build it using CMake and run the EXE

``` batch
if not exist out (mkdir out)
cd out
cmake ..
cmake --build .
cd Debug
start pong.exe
```

</details>
<details>
<summary>buildLin.sh</summary>
This shell checks if the build folder exists, if it doesn't it will make it. It will build it using CMake and run the application

``` shell
if [ ! -d "out" ]; then sudo mkdir out;
fi
cd out
sudo cmake ..
sudo cmake --build .
./pong
```

</details>

# Credit:

Music provided by JJD, promoted by NCMFYT <br>
Track: JJD - Nyan Cat <br>
Listen on YouTube: https://bit.ly/3SoHdz6 <br>
Stream: https://fanlink.tv/NyanCat
