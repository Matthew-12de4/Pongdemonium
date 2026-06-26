#pragma once
#pragma warning(disable : 4996) // Disables warning about using ctime for date and time leaderboard

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <mutex>
#include <vector>
#include "json.hpp"
#include "Ball.h"
#include "Button.h"
#include "Timer.h"
#include "Chaos.h"

using json = nlohmann::ordered_json;

class GameEngine
{
private:
	// Base code:
	sf::RenderWindow& m_window;
	sf::Font m_fontTex;
	sf::Font m_fontNum;
	sf::Text m_hud;
	sf::Text m_title;
	sf::SoundBuffer m_ballBuffer;
	sf::Sound m_ballSound;
	sf::Clock m_clock;
	sf::SoundBuffer m_stateBuffer;
	sf::Sound m_stateSound;

	//Text Handles
	sf::RectangleShape m_textField;
	sf::Text m_textFieldText;

	std::string m_boardName;
	std::random_device rd;

	// Paddle and ball
	Paddle m_paddle1;
	Paddle m_paddle2;
	Ball m_ball;
	sf::Vector2f m_paddleSize;
	sf::Vector2f m_paddlePos;
	float m_ballSize = 0;
	float m_paddleSpeed = 0;
	float m_basePaddleSpeed = 0;
	float m_ballSpeed = 0;
	bool m_paddle1Hit = false;
	bool m_paddle2Hit = false;

	// Main Menu buttons
	json optionsJS;
	Button m_topButton;
	Button m_middleButton;
	Button m_bottomButton;
	Button m_leaderButton;
	Button m_closeButton;

	// Options
	sf::RectangleShape m_sliderLine;
	sf::CircleShape m_sliderBall;
	sf::Text m_volumeText;
	Button m_checkFlashingLights;
	bool m_flashingLights;
	sf::Text m_flashingLightsTex;

	// Mode Select
	sf::Text m_currentMode;
	Button m_timedButton;
	Button m_scoredButton;
	Button m_endlessButton;

	// Positions on leaderboard
	Button m_leaderPos1;
	Button m_leaderPos2;
	Button m_leaderPos3;
	Button m_leaderPos4;
	Button m_leaderPos5;

	// Leaderboard
	sf::Text m_detailsLeader;
	sf::Text m_stats;
	std::string m_timeAliveDisplay;
	bool m_hasGameEnded = false;
	double m_timeAlive = 0;
	std::string m_minSec;

	json LeaderboardEnd;
	json LeaderboardTime;
	json LeaderboardScore;

	std::vector<int> scoreVec;
	std::vector<std::string> nameVec;

	//Chaos Effects
	Timer m_timer;
	sf::Color m_fromTimerColour;
	sf::Color m_toTimerColour;
	
	sf::Color m_baseFromTimerColour;
	sf::Color m_baseToTimerColour;
	sf::Color m_invFromTimerColour;
	sf::Color m_invToTimerColour;
	std::vector<sf::Color> m_baseVecColour;
	std::vector<sf::Color> m_invVecColours;

	Chaos m_chaos;
	sf::Music m_nyanMusic;
	std::vector<sf::Color> m_vecColours;
	sf::Color m_backColour;
	int m_chaosStat = 0;
	bool m_fakeCrash = false;
	std::vector<Ball> m_ballVec;
	sf::RectangleShape m_squareBall;
	std::vector<ChaosEffects> m_metaEffects;
	// Rick Roll
	std::vector<std::string> m_rickRoll;
	std::vector<sf::Image> m_rickRollIm;
	sf::Texture m_rickTexture;
	sf::Sprite m_rickSprite;
	sf::Music m_rickMusic;
	// Cats
	sf::Texture m_catBackTexture;
	sf::Sprite m_catBackSprite;
	sf::Texture m_catBallTexture;
	sf::Sprite m_catBallSprite;
	sf::Texture m_catPaddleTexture;
	sf::Sprite m_catPaddleSprite1;
	sf::Sprite m_catPaddleSprite2;
	// Extra Sound Effects
	sf::Music m_dvdMusic;
	sf::Music m_jesusMusic;
	sf::Music m_8bitMusic;

	// Mouse
	int m_mouseDelta = 0;
	float m_scrollSensitivity = 0;

	// Gameplay
	int m_timerDuration = 0;
	float m_aiFOV = 0;

	// Score
	int m_scoreIncrease = 0;
	int m_p1Score = 0;
	int m_p2Score = 0;

	// Limits
	static constexpr int m_maxScore = 50;
	static constexpr int m_maxTime = 65; // In seconds

	// States/ Modes
	enum GameMode {timed, scored, endless};
	GameMode m_gMode;
	enum GameStates { intro, options, textInput, leaderboard, detailedBoard, playing, gameOver };
	GameStates m_gStates;
public:
	GameEngine(sf::RenderWindow& window);
	// Utilities
	int generateRandom(int min, int max);
	void stopMusic();

	// Handling Inputs
	void handleInput(float dt);
	void handleButtons();

	// AI
	void ai(float dt, int& accuracy);
	void takeTheWheel(float dt, int& accuracy);

	// SFML Utilities
	void draw();
	void update();

	//Scenes
	void run();
	void mainMenu();
	void optionsMenu();
	void gameOverScreen();
	std::string textHandle();
	void leaderboardScreen();
	void detailLeaderboardScreen(int buttonPressed);
	void sceneManager();

};