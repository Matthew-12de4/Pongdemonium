#include "GameEngine.h"
#include <algorithm>
#include "Paddle.h"

GameEngine::GameEngine(sf::RenderWindow& window)
	: m_window(window),
	m_hud(m_fontNum),
	m_title(m_fontTex),
	m_ballSound(m_ballBuffer),
	m_stateSound(m_stateBuffer),
	m_textField({ float(m_window.getSize().x) - 100, 150 }),
	m_textFieldText(m_fontTex),
	m_paddle1(sf::Vector2f(20, float(window.getSize().y) / 2.f), 10, 100, sf::Color::White),
	m_paddle2(sf::Vector2f(float(window.getSize().x) - 20.f, float(window.getSize().y) - 100.f), 10, 100, sf::Color::White),
	m_ball(sf::Vector2f(float(window.getSize().x) / 2.f, float(window.getSize().y) / 2.f), 8, 400.f, sf::Color::White),
	m_topButton(sf::Vector2f(float(window.getSize().x) / 2.f, (float(window.getSize().y) / 2.f) - 125), "Play", 42, "./assets/images/Pong Button.png"),
	m_middleButton(sf::Vector2f(float(window.getSize().x) / 2.f, (float(window.getSize().y) / 2.f)), "Options", 42, "./assets/images/Pong Button.png"),
	m_bottomButton(sf::Vector2f(float(window.getSize().x) / 2.f, (float(window.getSize().y) / 2.f) + 125), "Quit", 42, "./assets/images/Pong Button.png"),
	m_leaderButton(sf::Vector2f((float(window.getSize().x) / 2.f) - 300, (float(window.getSize().y) / 2.f) + 200), "", 42, "./assets/images/leaderButton.png"),
	m_closeButton({ 60, 60 }, "", 1, "./assets/images/closeButton.png"),
	m_volumeText(m_fontTex),
	m_checkFlashingLights(sf::Vector2f((float(window.getSize().x) / 2.f) + 200, (float(window.getSize().y) / 2.f)), "./assets/images/unCheckedButton.png", "./assets/images/CheckedButton.png", false),
	m_flashingLightsTex(m_fontTex),
	m_currentMode(m_fontTex),
	m_timedButton({ (float(window.getSize().x) / 2.f) - 300, (float(m_window.getSize().y) / 2.f)}, "", 1, "./assets/images/timeBased.png"),
	m_scoredButton({ (float(window.getSize().x) / 2.f) - 300, (float(m_window.getSize().y) / 2.f) + 100 }, "", 1, "./assets/images/scoreBased.png"),
	m_endlessButton({ (float(window.getSize().x) / 2.f) - 300, (float(m_window.getSize().y) / 2.f) - 100 }, "", 1, "./assets/images/endlessBased.png"),
	m_leaderPos1(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 - 150), float(m_window.getSize().x - 50), 50, sf::Color(16, 173, 0), "Position1", 24),
	m_leaderPos2(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 - 50), float(m_window.getSize().x - 50), 50, sf::Color(16, 173, 0), "Position2", 24),
	m_leaderPos3(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 + 50), float(m_window.getSize().x - 50), 50, sf::Color(16, 173, 0), "Position3", 24),
	m_leaderPos4(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 + 150), float(m_window.getSize().x - 50), 50, sf::Color(16, 173, 0), "Position4", 24),
	m_leaderPos5(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 + 250), float(m_window.getSize().x - 50), 50, sf::Color(16, 173, 0), "Position5", 24),
	m_detailsLeader(m_fontTex),
	m_stats(m_fontTex),
	m_timer(sf::Vector2f(float(window.getSize().x) / 2.f, 45.f / 2.f), 400, 45, sf::Color(100, 100, 100, 127), m_chaos),
	m_chaos(m_window, m_paddleSpeed, m_paddle1, m_paddle2, m_ball, m_ballVec),
	m_squareBall({ m_ball.getSize() * 2, m_ball.getSize() * 2 }),
	m_rickSprite(m_rickTexture),
	m_catBackSprite(m_catBackTexture),
	m_catBallSprite(m_catBallTexture),
	m_catPaddleSprite1(m_catPaddleTexture),
	m_catPaddleSprite2(m_catPaddleTexture),
	m_gMode(GameMode::endless),
	m_gStates(GameStates::intro)
{
	// Icon
	sf::Image icon;
	if (!(icon.loadFromFile("./assets/images/Pong Icon White.png")))
		return;
	m_window.setIcon(icon.getSize(), icon.getPixelsPtr());

	// Mouse Def
	m_mouseDelta = 0;
	m_scrollSensitivity = 75; //25 - 75 good range

	// Range of AI View
	m_aiFOV = float(m_window.getSize().y) / 2.f;

	// Leaderboard stuff
	m_timerDuration = 10;
	m_scoreIncrease = 1;
	m_p1Score = 0;
	m_p2Score = 0;
	m_hasGameEnded = false;
	m_timeAlive = 0;
	m_minSec = "s";
	m_chaosStat = 0;

	//Setting ball/paddle variables
	m_paddle1Hit = false;
	m_paddle2Hit = false;
	m_ballSpeed = 400.f;
	m_basePaddleSpeed = 500.f;
	m_paddleSpeed = m_basePaddleSpeed;
	m_chaos.setBasePaddleSpeed(m_basePaddleSpeed);
	m_chaos.setBaseBallSpeed(m_ballSpeed);
	m_paddleSize = m_paddle1.getSize();
	m_ballSize = m_ball.getShape().getRadius();
	m_paddlePos = m_paddle1.getShape().getPosition();
	m_ball.setPosition(float(m_window.getSize().x) / 2, (float(m_window.getSize().y) / 2) + float(generateRandom(-100, 100)));

	//Sounds
	if (!m_ballBuffer.loadFromFile("./assets/audio/beepMono.ogg"))
		return;
	if (!m_stateBuffer.loadFromFile("./assets/audio/start.ogg"))
		return;

	//Fonts
	if (!(m_fontNum.openFromFile("./assets/fonts/digital-7.ttf"))) // Timer Font
		return;
	if (!(m_fontTex.openFromFile("./assets/fonts/arial_black.ttf")))
		return;

	m_backColour = sf::Color::Black;

	// Chaos
	m_baseFromTimerColour = sf::Color(250, 60, 250);
	m_baseToTimerColour = sf::Color::Red;
	m_invFromTimerColour = sf::Color(5, 195, 5);
	m_invToTimerColour = sf::Color(0, 255, 255);
	
	m_fromTimerColour = m_baseFromTimerColour;
	m_toTimerColour = m_baseToTimerColour;
	
	m_baseVecColour = { sf::Color::Red, sf::Color::Blue, sf::Color::Yellow, sf::Color::Cyan, sf::Color::Green, sf::Color::Magenta, sf::Color(223, 3, 252), sf::Color(252, 119, 3) };
	m_invVecColours = {sf::Color(0,255,255), sf::Color(255, 255, 0), sf::Color(0, 0, 255), sf::Color(255,0,0), sf::Color(255,0,255), sf::Color(0, 255, 0), sf::Color(32, 252, 3), sf::Color(252,119,3)};
	
	m_vecColours = m_baseVecColour;
	
	m_squareBall.setFillColor(sf::Color::White);
	
	// RickRoll GIF
	for (int i = 0; i < 23; i++)
	{
		std::stringstream ss;
		ss << "./assets/images/rickRoll/frame_" << i <<".gif";
		m_rickRoll.emplace_back(ss.str());
	}
	sf::Image f0(m_rickRoll[0]);
	sf::Image f1(m_rickRoll[1]);
	sf::Image f2(m_rickRoll[2]);
	sf::Image f3(m_rickRoll[3]);
	sf::Image f4(m_rickRoll[4]);
	sf::Image f5(m_rickRoll[5]);
	sf::Image f6(m_rickRoll[6]);
	sf::Image f7(m_rickRoll[7]);
	sf::Image f8(m_rickRoll[8]);
	sf::Image f9(m_rickRoll[9]);
	sf::Image f10(m_rickRoll[10]);
	sf::Image f11(m_rickRoll[11]);
	sf::Image f12(m_rickRoll[12]);
	sf::Image f13(m_rickRoll[13]);
	sf::Image f14(m_rickRoll[14]);
	sf::Image f15(m_rickRoll[15]);
	sf::Image f16(m_rickRoll[16]);
	sf::Image f17(m_rickRoll[17]);
	sf::Image f18(m_rickRoll[18]);
	sf::Image f19(m_rickRoll[19]);
	sf::Image f20(m_rickRoll[20]);
	sf::Image f21(m_rickRoll[21]);
	sf::Image f22(m_rickRoll[22]);
	m_rickRollIm = {f0, f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22};
	
	if (!m_rickTexture.loadFromImage(m_rickRollIm[0]))
		return;
	m_rickSprite.setTexture(m_rickTexture);
	m_rickSprite.setTextureRect(sf::IntRect({ { 0, 0 }, { 32, 32 } }));
	m_rickSprite.setOrigin(m_rickSprite.getLocalBounds().getCenter());
	
	// Cat Chaos Effect
	
	//Background
	if (!m_catBackTexture.loadFromFile("./assets/images/cat/catBack.png"))
		return;
	m_catBackSprite.setTexture(m_catBackTexture);
	m_catBackSprite.setTextureRect(sf::IntRect({ { 0, 0 }, { 800, 600 } }));
	// Ball
	if (!m_catBallTexture.loadFromFile("./assets/images/cat/catBall.png"))
		return;
	m_catBallSprite.setTexture(m_catBallTexture);
	m_catBallSprite.setTextureRect(sf::IntRect({ { 0, 0 }, { 32, 32 } }));
	m_catBallSprite.setOrigin(m_catBallSprite.getLocalBounds().getCenter());
	// Paddle
	if (!m_catPaddleTexture.loadFromFile("./assets/images/cat/catPaddle.png"))
		return;
	m_catPaddleSprite1.setTexture(m_catPaddleTexture);
	m_catPaddleSprite1.setTextureRect(sf::IntRect({ { 0, 0 }, { 70, 100 } }));
	m_catPaddleSprite1.setOrigin(m_catPaddleSprite1.getLocalBounds().getCenter());
	
	m_catPaddleSprite2.setTexture(m_catPaddleTexture);
	m_catPaddleSprite2.setTextureRect(sf::IntRect({ { 0, 0 }, { 70, 100 } }));
	m_catPaddleSprite2.setOrigin(m_catPaddleSprite2.getLocalBounds().getCenter());
	m_catPaddleSprite2.setScale({ -1, 1 });
	
	// Audio
	if (!m_rickMusic.openFromFile("./assets/audio/Never Gonna Give You Up 8Bit.mp3"))
		return;
	if (!m_nyanMusic.openFromFile("./assets/audio/Nyan Cat Theme.mp3"))
		return;
	if (!m_dvdMusic.openFromFile("./assets/audio/Dizzy.ogg"))
		return;
	if (!m_jesusMusic.openFromFile("./assets/audio/Heavenly Music.mp3"))
		return;
	if (!m_8bitMusic.openFromFile("./assets/audio/8bit.mp3"))
		return;
	
	// Options
	std::fstream fileWrite;
	std::fstream fileRead;

	if (!std::filesystem::exists("./assets/json/Options.json"))
	{
		fileWrite.open("./assets/json/Options.json", std::ios::out);
		optionsJS["volume"] = 100.f;
		optionsJS["flashingLights"] = m_checkFlashingLights.isChecked();

		fileWrite << std::setw(4) << optionsJS;
		fileWrite.close();
	}
	else
	{
		fileRead.open("./assets/json/Options.json", std::ios::in);

		json OptionsPar = json::parse(fileRead);

		sf::Listener::setGlobalVolume(OptionsPar.at("volume"));
		m_flashingLights = OptionsPar.at("flashingLights");
		m_checkFlashingLights.changeState(m_flashingLights);
		fileRead.close();
	}

	// Volume Shapes
	m_sliderLine.setSize({300, 5});
	m_sliderLine.setFillColor(sf::Color::White);
	m_sliderLine.setOrigin(m_sliderLine.getLocalBounds().getCenter());
	m_sliderLine.setPosition({ float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 - 80 });
	m_sliderBall.setRadius(10);
	m_sliderBall.setFillColor(sf::Color(250, 250, 250));
	m_sliderBall.setOrigin(m_sliderBall.getLocalBounds().getCenter());

	fileRead.open("./assets/json/Options.json", std::ios::in);

	json OptionsPar = json::parse(fileRead);
	m_sliderBall.setPosition({ (m_sliderLine.getPosition().x - m_sliderLine.getSize().x / 2) + ((m_sliderLine.getPosition().x + m_sliderLine.getSize().x / 2) - (m_sliderLine.getPosition().x - m_sliderLine.getSize().x / 2)) * (float(OptionsPar.at("volume")) / 100.f), m_sliderLine.getPosition().y});
	fileRead.close();

	//Text definitions
	m_hud.setFont(m_fontNum);
	m_hud.setCharacterSize(50);
	m_hud.setFillColor(sf::Color::White);
	m_hud.setLineAlignment(sf::Text::LineAlignment::Center);
	m_hud.setPosition({ (float(m_window.getSize().x) / 2.f), 40 });
	m_paddle1.setSpeed(m_paddleSpeed);

	m_title.setFont(m_fontTex);
	m_title.setString("Pong");
	m_title.setCharacterSize(64);
	m_title.setFillColor(sf::Color::White);
	m_title.setLineAlignment(sf::Text::LineAlignment::Center);
	m_title.setPosition(sf::Vector2f(float(m_window.getSize().x) / 2, 30 ));

	m_stats.setFont(m_fontTex);
	m_stats.setString("Player Score: \nEnemy Score: \nTime Alive: \nChaos Effects Survived: ");
	m_stats.setCharacterSize(42);
	m_stats.setFillColor(sf::Color::White);
	m_stats.setLineAlignment(sf::Text::LineAlignment::Left);
	m_stats.setOrigin(m_stats.getLocalBounds().getCenter());
	m_stats.setPosition(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 ));

	m_textField.setOrigin(m_textField.getLocalBounds().getCenter());
	m_textField.setPosition({ float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 });
	m_textField.setFillColor(sf::Color(50, 50, 50));
	m_textFieldText.setFont(m_fontTex);
	m_textFieldText.setString("");
	m_textFieldText.setCharacterSize(42);
	m_textFieldText.setFillColor(sf::Color::White);
	m_textFieldText.setLineAlignment(sf::Text::LineAlignment::Center);
	m_textFieldText.setPosition({ float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 });

	m_detailsLeader.setFont(m_fontTex);
	m_detailsLeader.setString("Detail");
	m_detailsLeader.setCharacterSize(45);
	m_detailsLeader.setFillColor(sf::Color::White);
	m_detailsLeader.setLineAlignment(sf::Text::LineAlignment::Center);
	m_detailsLeader.setPosition({ float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 });

	m_currentMode.setFont(m_fontTex);
	m_currentMode.setString("Current Mode:\nEndless");
	m_currentMode.setCharacterSize(25);
	m_currentMode.setFillColor(sf::Color::White);
	m_currentMode.setLineAlignment(sf::Text::LineAlignment::Center);
	m_currentMode.setPosition({ (float(window.getSize().x) / 2.f) - 300, (float(m_window.getSize().y) / 2.f) - 250 });

	m_volumeText.setFont(m_fontTex);
	m_volumeText.setString("Volume");
	m_volumeText.setCharacterSize(42);
	m_volumeText.setFillColor(sf::Color::White);
	m_volumeText.setLineAlignment(sf::Text::LineAlignment::Center);
	m_volumeText.setPosition({ m_sliderLine.getPosition().x, m_sliderLine.getPosition().y - 75 });

	m_flashingLightsTex.setFont(m_fontTex);
	m_flashingLightsTex.setString("Flashing Lights");
	m_flashingLightsTex.setCharacterSize(42);
	m_flashingLightsTex.setFillColor(sf::Color::White);
	m_flashingLightsTex.setLineAlignment(sf::Text::LineAlignment::Center);
	m_flashingLightsTex.setOrigin(m_flashingLightsTex.getLocalBounds().getCenter());
	m_flashingLightsTex.setPosition({ (m_checkFlashingLights.getShape().getPosition().x) - 250, m_checkFlashingLights.getShape().getPosition().y});
}

int GameEngine::generateRandom(int min, int max)
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

void GameEngine::stopMusic()
{
	m_nyanMusic.stop();
	m_rickMusic.stop();
	m_dvdMusic.stop();
	m_jesusMusic.stop();
	m_8bitMusic.stop();
}

void GameEngine::draw()
{
	// Chaos Effect (m_fakeCrash)
	if (!(m_fakeCrash))
	{
		// Draws what it needs to depend on the state
		switch (m_gStates)
		{
		case (GameStates::playing):
			if (m_chaos.getChaos() != ChaosEffects::tripping
				|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::tripping) > 0))
				m_window.clear(m_backColour);
			if (m_chaos.getChaos() == ChaosEffects::meow) m_window.draw(m_catBackSprite);
			if (m_chaos.getChaos() != ChaosEffects::meow)
			{
				m_paddle1.draw(m_window);
				m_paddle2.draw(m_window);
			}
			if (m_chaos.getChaos() == ChaosEffects::meow)
			{
				m_window.draw(m_catBallSprite);
				m_window.draw(m_catPaddleSprite1);
				m_window.draw(m_catPaddleSprite2);
			}
			if (m_chaos.getChaos() == ChaosEffects::squareBall)
				m_window.draw(m_squareBall);
			if (!(m_chaos.getChaos() == ChaosEffects::squareBall || m_chaos.getChaos() == ChaosEffects::rickBall || m_chaos.getChaos() == ChaosEffects::meow))
				m_ball.draw(m_window);
			m_timer.draw(m_window);
			m_chaos.draw(m_window);
			m_window.draw(m_hud);
			if (m_chaos.getChaos() == ChaosEffects::manyBalls
				|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0))
			{
				for (auto & i : m_ballVec)
				{
					i.draw(m_window);
				}
			}
			if (m_chaos.getChaos() == ChaosEffects::rickBall)
				m_window.draw(m_rickSprite);
			m_window.display();
			break;
		case (GameStates::intro):
			m_window.clear(m_backColour);
			m_window.draw(m_title);
			m_topButton.draw(m_window);
			m_middleButton.draw(m_window);
			m_bottomButton.draw(m_window);
			m_leaderButton.draw(m_window);
			m_timedButton.draw(m_window);
			m_scoredButton.draw(m_window);
			m_endlessButton.draw(m_window);
			m_window.draw(m_currentMode);
			m_window.display();
			break;
		case (GameStates::options):
			m_window.clear(m_backColour);
			m_window.draw(m_title);
			m_closeButton.draw(m_window);
			m_window.draw(m_sliderLine);
			m_window.draw(m_sliderBall);
			m_window.draw(m_volumeText);
			m_checkFlashingLights.draw(m_window);
			m_window.draw(m_flashingLightsTex);
			m_window.display();
			break;
		case (GameStates::gameOver):
			m_window.clear(m_backColour);
			m_window.draw(m_title);
			m_window.draw(m_stats);
			m_bottomButton.draw(m_window);
			m_window.display();
			break;
		case (GameStates::textInput):
			m_window.clear(m_backColour);
			m_window.draw(m_title);
			m_window.draw(m_textField);
			m_window.draw(m_textFieldText);
			m_closeButton.draw(m_window);
			m_bottomButton.draw(m_window);
			m_window.display();
			break;
		case (GameStates::leaderboard):
			m_window.clear(m_backColour);
			m_window.draw(m_title);
			m_leaderPos1.draw(m_window);
			m_leaderPos2.draw(m_window);
			m_leaderPos3.draw(m_window);
			m_leaderPos4.draw(m_window);
			m_leaderPos5.draw(m_window);
			m_closeButton.draw(m_window);
			m_window.display();
			break;
		case (GameStates::detailedBoard):
			m_window.clear(m_backColour);
			m_window.draw(m_title);
			m_window.draw(m_detailsLeader);
			m_closeButton.draw(m_window);
			m_window.display();
			break;
		}
	}
}

void GameEngine::update()
{
	// update hud
	std::stringstream ss;
	switch (m_gStates)
	{
	case GameEngine::intro:
		ss << "Press the Space\nkey to start";
		break;
	case GameEngine::playing:
		ss << m_p1Score << " - " << m_p2Score;
		break;
	case GameEngine::gameOver:
		if (m_p1Score > m_p2Score) {
			ss << "Player 1 wins\nPress space to restart";
		}
		else {
			ss << "Player 2 wins\nPress space to restart";
		}
		break;
	default:
		break;
	}

	m_hud.setString(ss.str());
}

// Input Handling

void GameEngine::handleButtons()
{
	switch (m_gStates)
	{
	case GameStates::intro:
		if (m_topButton.isClicked(m_window))
		{
			m_gStates = GameStates::playing;
			sceneManager();
		}
		if (m_middleButton.isClicked(m_window))
		{
			m_gStates = GameStates::options;
			sceneManager();
		}
		if (m_leaderButton.isClicked(m_window))
		{
			m_gStates = GameStates::leaderboard;
			sceneManager();
		}
		if (m_bottomButton.isClicked(m_window))
		{
			m_window.close();
		}
		if (m_endlessButton.isClicked(m_window))
		{
			m_gMode = GameMode::endless;
			m_currentMode.setString("Current Mode:\nEndless");
		}
		if (m_timedButton.isClicked(m_window))
		{
			m_gMode = GameMode::timed;
			m_currentMode.setString("Current Mode:\nTimed");
		}
		if (m_scoredButton.isClicked(m_window))
		{
			m_gMode = GameMode::scored;
			m_currentMode.setString("Current Mode:\nScored");
		}
		break;

	case GameStates::options:
		if (m_closeButton.isClicked(m_window))
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}
		if (m_checkFlashingLights.isClickedCheckbox(m_window))
		{
			m_flashingLights = true;
		}
		else
			m_flashingLights = false;
		break;
	case GameStates::gameOver:
		if (m_bottomButton.isClicked(m_window))
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}
		break;
	case GameStates::leaderboard:
		if (m_leaderPos1.isClicked(m_window))
		{
			detailLeaderboardScreen(1);
		}
		if (m_leaderPos2.isClicked(m_window))
		{
			detailLeaderboardScreen(2);
		}
		if (m_leaderPos3.isClicked(m_window))
		{
			detailLeaderboardScreen(3);
		}
		if (m_leaderPos4.isClicked(m_window))
		{
			detailLeaderboardScreen(4);
		}
		if (m_leaderPos5.isClicked(m_window))
		{
			detailLeaderboardScreen(5);
		}
		if (m_closeButton.isClicked(m_window))
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}
		break;
	case GameStates::detailedBoard:
		if (m_closeButton.isClicked(m_window))
		{
			m_gStates = GameStates::leaderboard;
			sceneManager();
		}
		break;
	case GameStates::textInput:
		if (m_closeButton.isClicked(m_window))
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}
		break;
	default:
		break;
	}
}

void GameEngine::handleInput(float dt)
{
	switch (m_gStates)
	{
	case (GameStates::intro):
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space) && m_window.hasFocus())
		{
			m_gStates = GameStates::playing;
			sceneManager();
		}
		// Switching Game Modes
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num1) && m_window.hasFocus())
		{
			m_gMode = GameMode::endless;
			m_currentMode.setString("Current Mode:\nEndless");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num2) && m_window.hasFocus())
		{
			m_gMode = GameMode::timed;
			m_currentMode.setString("Current Mode:\nTimed");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Num3) && m_window.hasFocus())
		{
			m_gMode = GameMode::scored;
			m_currentMode.setString("Current Mode:\nScored");
		}
		break;

	case (GameStates::playing):
		// Switch Sides Input
		if (m_chaos.getChaos() == ChaosEffects::switchingSides)
		{
			m_paddle2.setSpeed(m_paddleSpeed);

			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel) // UP
				m_paddle2.move(dt, 0);
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) && m_window.hasFocus())) && m_chaos.getChaos() != ChaosEffects::jesusWheel) // Down
				m_paddle2.move(dt, float(m_window.getSize().y));
			// Scroll Wheel
			if ((m_mouseDelta > 0 && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel)
			{
				m_paddle2.setSpeed(m_paddleSpeed * m_scrollSensitivity);
				m_paddle2.move(dt, 0);
				m_paddle2.setSpeed(m_paddleSpeed);
			}
			else if ((m_mouseDelta < 0 && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel)
			{
				m_paddle2.setSpeed(m_paddleSpeed * m_scrollSensitivity);
				m_paddle2.move(dt, float(m_window.getSize().y));
				m_paddle2.setSpeed(m_paddleSpeed);
			}
		}
		// Move Anywhere Input
		else if (m_chaos.getChaos() == ChaosEffects::moveAnywhere
			|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::moveAnywhere) > 0))
		{
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A)) && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel)
				m_paddle1.moveLat(dt, 0);
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D)) && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel)
				m_paddle1.moveLat(dt, float(m_window.getSize().x));
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel) // UP
				m_paddle1.move(dt, 0);
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) && m_window.hasFocus())) && m_chaos.getChaos() != ChaosEffects::jesusWheel) // Down
				m_paddle1.move(dt, float(m_window.getSize().y));
		}
		// Basic Input
		else if (m_chaos.getChaos() != ChaosEffects::invertControls)
		{
			m_paddle1.setSpeed(m_paddleSpeed);

			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel) // UP
				m_paddle1.move(dt, 0);
			if (((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) && m_window.hasFocus())) && m_chaos.getChaos() != ChaosEffects::jesusWheel) // Down
				m_paddle1.move(dt, float(m_window.getSize().y));
			// Scroll Wheel
			if ((m_mouseDelta > 0 && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel)
			{
				m_paddle1.setSpeed(m_paddleSpeed * m_scrollSensitivity);
				m_paddle1.move(dt, 0);
				m_paddle1.setSpeed(m_paddleSpeed);
			}
			else if ((m_mouseDelta < 0 && m_window.hasFocus()) && m_chaos.getChaos() != ChaosEffects::jesusWheel)
			{
				m_paddle1.setSpeed(m_paddleSpeed * m_scrollSensitivity);
				m_paddle1.move(dt, float(m_window.getSize().y));
				m_paddle1.setSpeed(m_paddleSpeed);
			}
		}
		// Inverted Controls
		else
		{
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) && m_window.hasFocus()) // UP
				m_paddle1.move(dt, 0);
			if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) && m_window.hasFocus()) // Down
				m_paddle1.move(dt, float(m_window.getSize().y));
			if (m_mouseDelta < 0 && m_window.hasFocus())
			{
				m_paddle1.setSpeed(m_paddleSpeed * m_scrollSensitivity);
				m_paddle1.move(dt, 0);
				m_paddle1.setSpeed(m_paddleSpeed);

			}
			else if (m_mouseDelta > 0 && m_window.hasFocus())
			{
				m_paddle1.setSpeed(m_paddleSpeed * m_scrollSensitivity);
				m_paddle1.move(dt, float(m_window.getSize().y));
				m_paddle1.setSpeed(m_paddleSpeed);
			}
		}
		break;

		default:
			break;
	}
}

// AI
void GameEngine::ai(float dt, int& accuracy)
{
	int random = generateRandom(1, accuracy);
	int negNum = generateRandom(0, 1);

	if (negNum)
		random *= -1;

	float ballRange = m_ball.getPosition().y + float(random);

	m_paddle2.setSpeed(m_paddleSpeed);

	// Chaos: Aim bot 
	if (m_chaos.getChaos() == ChaosEffects::aiAimBot
		|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::aiAimBot) > 0))
	{
		if (std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0)
		{
			std::vector<sf::Vector2f> ballXVal;

			for (auto & i : m_ballVec)
			{
				ballXVal.emplace_back( i.getPosition().x, i.getPosition().y );
			}
			std::sort(ballXVal.begin(), ballXVal.end(), [](const sf::Vector2f& a, const sf::Vector2f& b)
				{ return a.x > b.x; });

			for (int i = 0; i < m_ballVec.size(); i++)
			{
				if (ballXVal[0].x >= float(m_window.getSize().x) - 50)
				{
					m_paddle2.setPosition({ m_paddle2.getShape().getPosition().x, ballXVal[0].y });
				}
				else
				{
					m_paddle2.setPosition({ m_paddle2.getShape().getPosition().x, float(m_window.getSize().y) / 2 });
				}
			}
		}
		else if (m_ball.getPosition().x >= float(m_window.getSize().x) - 50)
		{
			m_paddle2.setPosition({ m_paddle2.getShape().getPosition().x, m_ball.getPosition().y });
		}
		else
		{
			m_paddle2.setPosition({ m_paddle2.getShape().getPosition().x, float(m_window.getSize().y) / 2 });
		}

	}

	// Chaos: Many balls
	else if (m_chaos.getChaos() == ChaosEffects::manyBalls
		|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0))
	{
		std::vector<sf::Vector2f> ballXVal;

		for (auto & i : m_ballVec)
		{
			ballXVal.emplace_back(sf::Vector2f(i.getPosition().x, i.getPosition().y ));
		}
		std::sort(ballXVal.begin(), ballXVal.end(), [](const sf::Vector2f& a, const sf::Vector2f& b)
			{ return a.x > b.x; });

		float manyBallRange = ballXVal[0].y + float(random);
		if (manyBallRange <= m_paddle2.getShape().getPosition().y)
		{
			m_paddle2.move(dt, 0);
		}

		else if (manyBallRange >= m_paddle2.getShape().getPosition().y)
		{
			m_paddle2.move(dt, float(m_window.getSize().y));
		}
	}
	// Chaos: Move Anywhere
	else if (m_ball.getPosition().x >= m_aiFOV || (m_chaos.getChaos() == ChaosEffects::moveAnywhere
		|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::moveAnywhere) > 0)))
	{
		if (!m_paddle2Hit || (m_chaos.getChaos() == ChaosEffects::moveAnywhere
			|| (m_chaos.getChaos() == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::moveAnywhere) > 0)))
		{
			if (ballRange <= m_paddle2.getShape().getPosition().y)
			{
				m_paddle2.move(dt, 0);
			}

			else if (ballRange >= m_paddle2.getShape().getPosition().y)
			{
				m_paddle2.move(dt, float(m_window.getSize().y));
			}
		}
		else
		{
			m_paddle2.move(dt, float(m_window.getSize().y) / 2);
		}
	}
	else
	{
		// Regenerates accuracy
		accuracy = generateRandom(1, 100);
		m_paddle2Hit = false;
	}
}
void GameEngine::takeTheWheel(float dt, int& accuracy)
{
	int random = generateRandom(1, accuracy);
	int negNum = generateRandom(0, 1);

	if (negNum)
		random *= -1;

	float ballRange = m_ball.getPosition().y + float(random);

	m_paddle1.setSpeed(m_paddleSpeed);

	if (m_ball.getPosition().x <= float(m_window.getSize().x - 100))
	{
		if (!m_paddle1Hit)
		{
			if (ballRange <= m_paddle1.getShape().getPosition().y)
			{
				m_paddle1.move(dt, 0);
			}

			if (ballRange >= m_paddle1.getShape().getPosition().y)
			{
				m_paddle1.move(dt, float(m_window.getSize().y));
			}
		}
		else
		{
			m_paddle1.move(dt, float(m_window.getSize().y) / 2);
		}
	}
	else
	{
		accuracy = generateRandom(1, 100);
		m_paddle1Hit = false;
	}
}

// Scenes

void GameEngine::sceneManager()
{
	switch (m_gStates)
	{
	case(GameStates::intro):
		stopMusic();
		
		// Resetting
		m_backColour = sf::Color::Black;
		m_bottomButton.setPosition(sf::Vector2f(float(m_window.getSize().x) / 2.f, (float(m_window.getSize().y) / 2.f) + 125));
		m_bottomButton.setCharacterSize(42);
		m_bottomButton.setText("Quit");
		m_title.setString("Pong");
		m_title.setCharacterSize(64);
		m_hasGameEnded = false;
		mainMenu();
		break;
	case (GameStates::options):
		optionsMenu();
		break;
	case(GameStates::playing):
		stopMusic();
		
		// resetting colours
		m_hud.setFillColor(sf::Color::White);
		m_paddle1.setColour(sf::Color::White);
		m_paddle2.setColour(sf::Color::White);
		m_ball.setColour(sf::Color::White);
		m_backColour = sf::Color::Black;
		m_fromTimerColour = m_baseFromTimerColour;
		m_toTimerColour = m_baseToTimerColour;
		m_vecColours = m_baseVecColour;
		m_timer.setTextColour(sf::Color::White);
		
		// Type Name
		m_boardName = textHandle();

		if (!m_stateBuffer.loadFromFile("./assets/audio/start.ogg"))
			return;
		m_stateSound.play();

		// Resetting Everything Back
		m_chaosStat = 0;
		m_p1Score = 0;
		m_p2Score = 0;
		m_ballVec.clear();
		m_ball.setPosition(float(m_window.getSize().x) / 2, (float(m_window.getSize().y) / 2) + float(generateRandom(-100, 100)));
		m_ball.updateVelocity(1);
		m_ball.setSpeed(m_ballSpeed);
		m_ball.setSize(m_ballSize);
		m_paddleSpeed = m_basePaddleSpeed;
		m_paddle1.setSpeed(m_paddleSpeed);
		m_paddle2.setSpeed(m_paddleSpeed);
		m_paddle1.setSize(m_paddleSize);
		m_paddle1Hit = false;
		m_paddle2Hit = false;
		m_paddle1.setPosition(m_paddlePos);
		m_paddle2.setPosition({ m_paddle2.getShape().getPosition().x, float(m_window.getSize().y) / 2 });
		m_chaos.setText("No Chaos");
		m_chaos.setChaos(ChaosEffects::none);
		run();
		break;
	case(GameStates::gameOver):
		stopMusic();

		m_backColour = sf::Color::Black;
		m_bottomButton.setPosition(sf::Vector2f(float(m_window.getSize().x) / 2.f, (float(m_window.getSize().y) / 2.f) + 200));
		m_bottomButton.setCharacterSize(32);
		m_bottomButton.setText("Main Menu");
		
		// Leaderboard Maths
		if (m_timeAlive >= 60)
		{
			m_timeAlive /= 60;
			m_timeAlive = std::round(m_timeAlive * 100) / 100;
			m_minSec = "m";
		}
		gameOverScreen();
		break;
	case(GameStates::textInput):
		textHandle();
		break;
	case(GameStates::leaderboard):
		nameVec.clear();
		scoreVec.clear();
		leaderboardScreen();
		break;
	default:
		break;
	}
}
// Main Gameplay Loop
void GameEngine::run()
{
	float dt = m_clock.restart().asSeconds();
	float durationDt = 0;
	
	// Start Count Down For Different Things
	auto chaosStart = std::chrono::steady_clock::now();
	auto scoreStart = std::chrono::steady_clock::now();
	auto colourStart = std::chrono::steady_clock::now();
	auto rickStart = std::chrono::steady_clock::now();
	auto dvdStart = std::chrono::steady_clock::now();

	auto timeAlive = std::chrono::steady_clock::now();
	m_minSec = "s";

	ChaosEffects currentChaos = m_chaos.getChaos();
	int originalTimerDuration = m_timerDuration;
	int speedyDuration = 0;
	int frame = 0;
	sf::Vector2i windowVelocity = {1,1};

	int colourInc = 0;

	int accuracy = generateRandom(1, 75); // 65+ miss

	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();
		durationDt += dt;

		//startFPS = std::chrono::high_resolution_clock::now();

		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					if (m_gMode == GameMode::endless)
					{
						// Saves Score If In Endless
						std::chrono::duration<double> aliveSeconds = std::chrono::steady_clock::now() - timeAlive;
						int minutes = int(aliveSeconds.count()) / 60;
						int seconds = int(std::fmod(aliveSeconds.count(), 60)); // mod (%) for doubles
						std::stringstream ss;

						if (aliveSeconds.count() < 60)
						{
							ss << seconds << "s";
						}
						else
							ss << minutes << "m " << seconds << "s";

						m_timeAliveDisplay = ss.str();
						m_timeAlive = std::round(aliveSeconds.count() * 100.f) / 100.f;
						m_gStates = GameStates::gameOver;
						sceneManager();
					}
					else
					{
					m_gStates = GameStates::intro;
					sceneManager();
					}
				}
			}
			if (const auto* mousewheel = event->getIf<sf::Event::MouseWheelScrolled>()) {
				m_mouseDelta = int(mousewheel->delta);
				handleInput(dt);
				m_mouseDelta = 0;
			}
		}

		// Chaos Effects
		// Selecting Chaos Effects
		std::chrono::duration<double> elapsed_seconds = std::chrono::steady_clock::now() - chaosStart;
		if (elapsed_seconds.count() >= m_timerDuration)
		{
			// Resetting Things Back To Original State
			// Resetting Colours
			m_hud.setFillColor(sf::Color::White);
			m_paddle1.setColour(sf::Color::White);
			m_paddle2.setColour(sf::Color::White);
			m_ball.setColour(sf::Color::White);
			m_backColour = sf::Color::Black;
			m_fromTimerColour = m_baseFromTimerColour;
			m_toTimerColour = m_baseToTimerColour;
			m_vecColours = m_baseVecColour;
			m_timer.setTextColour(sf::Color::White);
			if (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0)
			{
				for (auto & i : m_ballVec)
				{
					i.setColour(sf::Color::Black);
				}
			}
			
			if (!m_ballBuffer.loadFromFile("./assets/audio/beepMono.ogg"))
				return;
			if (currentChaos == ChaosEffects::switchingSides)
			{
				int tempP1Score = m_p2Score;
				int tempP2Score = m_p1Score;
				m_p1Score = tempP1Score;
				m_p2Score = tempP2Score;
			}
			stopMusic();

			m_timerDuration = originalTimerDuration;
			m_backColour = sf::Color::Black;
			m_chaosStat++;
			
			m_chaos.selectChaos(m_aiFOV, m_flashingLights); // Chaos Effect Selected
			
			currentChaos = m_chaos.getChaos();
			chaosStart = std::chrono::steady_clock::now(); // Restart Timer
			
			// Chaos Effects Taking Place
			if (currentChaos == ChaosEffects::meta)
			{
				m_metaEffects = m_chaos.getMetaEffects();
			}
			if (currentChaos == ChaosEffects::manyBalls
				|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0))
			{
				for (auto & i : m_ballVec)
				{
					int randVel = generateRandom(0, 1);
					if (randVel == 0)
						randVel = -1;
					i.updateVelocity(float(1 * randVel));
				}
			}
			if (currentChaos == ChaosEffects::invertColour
				|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::invertColour) > 0))
			{
				m_hud.setFillColor(sf::Color::Black);
				m_paddle1.setColour(sf::Color::Black);
				m_paddle2.setColour(sf::Color::Black);
				m_ball.setColour(sf::Color::Black);
				m_backColour = sf::Color::White;
				m_fromTimerColour = m_invFromTimerColour;
				m_toTimerColour = m_invToTimerColour;
				m_vecColours = m_invVecColours;
				m_timer.setTextColour(sf::Color::Black);
				if (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0)
				{
					for (auto & i : m_ballVec)
					{
						i.setColour(sf::Color::White);
					}
				}
			}
			if (currentChaos == ChaosEffects::fakeCrash)
				m_fakeCrash = true;
			if (currentChaos == ChaosEffects::switchingSides)
			{
				int tempP1Score = m_p1Score;
				int tempP2Score = m_p2Score;
				m_p1Score = tempP2Score;
				m_p2Score = tempP1Score;
			}
			
			// Chaos Audio Effects
			if (currentChaos == ChaosEffects::meow)
			{
				if (!m_ballBuffer.loadFromFile("./assets/audio/catBounce.ogg"))
					return;
			}
			if (currentChaos == ChaosEffects::rickBall) m_rickMusic.play();
			if (currentChaos == ChaosEffects::ballColour || currentChaos == ChaosEffects::paddleColour || currentChaos == ChaosEffects::rapidBackgroundColour
				|| currentChaos == ChaosEffects::meow
				|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::ballColour) > 0)
				|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::paddleColour) > 0)
				|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::rapidBackgroundColour) > 0))
				m_nyanMusic.play();
			if (currentChaos == ChaosEffects::dvdWindow
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::dvdWindow) > 0))
				m_dvdMusic.play();
			if (currentChaos == ChaosEffects::jesusWheel)
				m_jesusMusic.play();
			if (currentChaos == ChaosEffects::laggy
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::laggy) > 0))
				m_8bitMusic.play();
			
			if (currentChaos == ChaosEffects::speedyTimer)
				speedyDuration += 5;
			if (speedyDuration > 0)
			{
				speedyDuration--;
				m_timerDuration = 2;
				std::cerr << speedyDuration << std::endl;
			}
		}
		
		// Chaos Effects On Tick
		if (currentChaos == ChaosEffects::dvdWindow
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::dvdWindow) > 0))
		{
			std::chrono::duration<double> ElapsedSeconds = std::chrono::steady_clock::now() - dvdStart;
			if (ElapsedSeconds.count() >= 0.005)
			{
				dvdStart = std::chrono::steady_clock::now();
				if (m_window.getPosition().y <= 30 || m_window.getPosition().y + m_window.getSize().y >= sf::VideoMode::getDesktopMode().size.y - 80)
				{
					windowVelocity.y = -windowVelocity.y;
				}
				if (m_window.getPosition().x <= 80 || m_window.getPosition().x + m_window.getSize().x >= sf::VideoMode::getDesktopMode().size.x)
				{
					windowVelocity.x = -windowVelocity.x;
				}
				m_window.setPosition(sf::Vector2i(m_window.getPosition().x + windowVelocity.x, m_window.getPosition().y + windowVelocity.y ));
			}
		}
		if (currentChaos == ChaosEffects::meow)
		{
			m_catBallSprite.setPosition(m_ball.getPosition());
			m_catPaddleSprite1.setPosition(m_paddle1.getShape().getPosition());
			m_catPaddleSprite2.setPosition(m_paddle2.getShape().getPosition());
		}
		if (currentChaos == ChaosEffects::squareBall)
		{
			m_squareBall.setPosition(m_ball.getPosition());
		}
		if (m_fakeCrash)
		{
			std::chrono::duration<double> fakeElapsedSeconds = std::chrono::steady_clock::now() - chaosStart;
			if (fakeElapsedSeconds.count() >= 1.5)
			{
				m_fakeCrash = false;
			}
		}
		if (currentChaos == ChaosEffects::jesusWheel || currentChaos == ChaosEffects::switchingSides)
			takeTheWheel(dt, accuracy);
		
		if (currentChaos == ChaosEffects::jitter
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::jitter) > 0))
		{
			m_ball.jitterMove(dt, m_window);
			if (std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0)
			{
				for (auto & i : m_ballVec)
				{
					i.jitterMove(dt, m_window);
				}
			}
		}
		else
			m_ball.move(dt, m_window); // Primary Ball Move Function

		if (currentChaos == ChaosEffects::manyBalls
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0))
		{
			for (auto & i : m_ballVec)
			{
				if (!(currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::jitter) > 0))
					i.move(dt, m_window);
				switch (i.checkWin(m_window))
				{
				case 1:
					if (!m_stateBuffer.loadFromFile("./assets/audio/eScore.ogg"))
						return;
					m_stateSound.play();
					m_p2Score += m_scoreIncrease;
					i.setPosition(float(m_window.getSize().x) / 2.f, (float(m_window.getSize().y) / 2.f) + float(generateRandom(-100, 100))); // Ball hits right side player scores
					i.updateVelocity(1);
					break;
				case 2:
					if (!m_stateBuffer.loadFromFile("./assets/audio/pScore.ogg"))
						return;
					m_stateSound.play();
					m_p1Score += m_scoreIncrease;
					i.setPosition(float(m_window.getSize().x) / 2.f, (float(m_window.getSize().y) / 2.f) + float(generateRandom(-100, 100))); // ball hits left side enemy scores
					i.updateVelocity(-1);
					break;
				default:
					break;
				}
				if (i.hitPaddle(m_paddle1, 1))
				{
					if (m_chaos.getChaos() == ChaosEffects::jesusWheel)
						m_paddle1Hit = true;
					i.updateVelocity(1);
					m_ballSound.play();
				}
				else if (i.hitPaddle(m_paddle2, 2))
				{
					m_paddle2Hit = true;
					i.updateVelocity(-1);
					m_ballSound.play();
				}
			}
		}
		if (currentChaos == ChaosEffects::paddleColour
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::paddleColour) > 0))
		{
			std::chrono::duration<double> elapsedColourSeconds = std::chrono::steady_clock::now() - colourStart;

			if (elapsedColourSeconds.count() >= 0.1)
			{
				m_paddle1.setColour(m_vecColours[colourInc]);
				m_paddle2.setColour(m_vecColours[colourInc]);
				colourInc++;
				if (colourInc >= m_vecColours.size())
					colourInc = 0;
				colourStart = std::chrono::steady_clock::now();
			}
		}
		if (currentChaos == ChaosEffects::ballColour
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::ballColour) > 0))
		{
			std::chrono::duration<double> elapsedColourSeconds = std::chrono::steady_clock::now() - colourStart;

			if (elapsedColourSeconds.count() >= 0.1)
			{
				if (std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::manyBalls) > 0)
				{
					for (auto & i : m_ballVec)
					{
						i.setColour(m_vecColours[colourInc]);
					}
				}
				m_ball.setColour(m_vecColours[colourInc]);
				colourInc++;
				if (colourInc >= m_vecColours.size())
					colourInc = 0;
				colourStart = std::chrono::steady_clock::now();
			}
		}
		if (currentChaos == ChaosEffects::rapidBackgroundColour
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::rapidBackgroundColour) > 0)) // play Nyan Cat music for duration of effect
		{
			std::chrono::duration<double> elapsedColourSeconds = std::chrono::steady_clock::now() - colourStart;

			if (elapsedColourSeconds.count() >= 0.2)
			{
				m_backColour = m_vecColours[colourInc];
				colourInc++;
				if (colourInc >= m_vecColours.size())
					colourInc = 0;
				colourStart = std::chrono::steady_clock::now();
			}
		}
		if (currentChaos == ChaosEffects::rickBall)
		{
			std::chrono::duration<double> elapsedRickSeconds = std::chrono::steady_clock::now() - rickStart;
			if (elapsedRickSeconds.count() >= 0.1)
			{
				if (!m_rickTexture.loadFromImage(m_rickRollIm[frame]))
					return;
				m_rickSprite.setTexture(m_rickTexture);
				frame++;
				if (frame >= m_rickRollIm.size()) frame = 0;
				rickStart = std::chrono::steady_clock::now();
			}
			m_rickSprite.setPosition(m_ball.getPosition());
		}

		// Checks if the ball has hit either side
		if (currentChaos == ChaosEffects::cannotWin
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::cannotWin) > 0))
		{
			if (m_ball.getPosition().x >= float(m_window.getSize().x)) m_ball.updateVelocity(-1);
			
			else if (m_ball.getPosition().x <= 0) m_ball.updateVelocity(1);
		}
		else
		{
			switch (m_ball.checkWin(m_window))
			{
			case 1:
				if (currentChaos == ChaosEffects::meow)
				{
					if (!m_stateBuffer.loadFromFile("./assets/audio/catEnemy.mp3"))
						return;
				}
				else
				{
					if (!m_stateBuffer.loadFromFile("./assets/audio/eScore.ogg"))
						return;
				}
				m_stateSound.play();
				m_p2Score += m_scoreIncrease;
				m_ball.setPosition(float(m_window.getSize().x) / 2.f, (float(m_window.getSize().y) / 2.f) + float(generateRandom(-300, 300))); // Ball hits right side player scores
				m_ball.updateVelocity(1);
				break;
			case 2:
				if (currentChaos == ChaosEffects::meow)
				{
					if (!m_stateBuffer.loadFromFile("./assets/audio/catPlayer.ogg"))
						return;
				}
				else
				{
					if (!m_stateBuffer.loadFromFile("./assets/audio/pScore.ogg"))
						return;
				}
				m_stateSound.play();
				m_p1Score += m_scoreIncrease;
				m_ball.setPosition(float(m_window.getSize().x) / 2.f, (float(m_window.getSize().y) / 2.f) + float(generateRandom(-300, 300))); // ball hits left side enemy scores
				m_ball.updateVelocity(-1);
				break;
			default:
				break;
			}
		}

		// Check If Hit Either Paddle
		if (currentChaos == ChaosEffects::rotatePaddle
			|| (currentChaos == ChaosEffects::meta && std::count(m_metaEffects.begin(), m_metaEffects.end(), ChaosEffects::rotatePaddle) > 0))
		{
			if (m_ball.hitPaddle(m_paddle1, 1, {int(m_paddle1.getSize().y) / 2, int(m_paddle1.getSize().x)}))
			{
				if (m_chaos.getChaos() == ChaosEffects::jesusWheel)
					m_paddle1Hit = true;
				m_ball.updateVelocity(1);
				m_ballSound.play();
			}
			else if (m_ball.hitPaddle(m_paddle2, 2, {int(m_paddle2.getSize().y) / 2, int(m_paddle2.getSize().x)}))
			{
				m_paddle2Hit = true;
				m_ball.updateVelocity(-1);
				m_ballSound.play();
			}
		}
		else
		{
			if (m_ball.hitPaddle(m_paddle1, 1))
			{
				if (m_chaos.getChaos() == ChaosEffects::jesusWheel)
					m_paddle1Hit = true;
				m_ball.updateVelocity(1);
				m_ballSound.play();
			}
			else if (m_ball.hitPaddle(m_paddle2, 2))
			{
				m_paddle2Hit = true;
				m_ball.updateVelocity(-1);
				m_ballSound.play();
			}
		}
		

		// Score handling
		// For Scored Mode
		if ((m_p1Score >= m_maxScore || m_p2Score >= m_maxScore) && m_gMode == GameMode::scored)
		{
			if (currentChaos == ChaosEffects::switchingSides)
			{
				int tempP1Score = m_p2Score;
				int tempP2Score = m_p1Score;
				m_p1Score = tempP1Score;
				m_p2Score = tempP2Score;
			}
			std::chrono::duration<double> aliveSeconds = std::chrono::steady_clock::now() - timeAlive;
			int minutes = int(aliveSeconds.count()) / 60;
			int seconds = int(std::fmod(aliveSeconds.count(), 60)); // mod (%) for doubles
			std::stringstream ss;

			if (aliveSeconds.count() < 60)
			{
				ss << seconds << "s";
			}
			else
				ss << minutes << "m " << seconds << "s";

			m_timeAliveDisplay = ss.str();
			m_timeAlive = std::round(aliveSeconds.count() * 100.f) / 100.f;
			m_gStates = GameStates::gameOver;
			sceneManager();
		}

		// For Timed Mode
		std::chrono::duration<double> scoredElapsedSeconds = std::chrono::steady_clock::now() - scoreStart;
		if (scoredElapsedSeconds.count() >= m_maxTime && m_gMode == GameMode::timed)
		{
			std::chrono::duration<double> aliveSeconds = std::chrono::steady_clock::now() - timeAlive;
			int seconds = int(std::fmod(aliveSeconds.count(), 60)); // mod % for doubles
			int minutes = int(aliveSeconds.count()) / 60;
			std::stringstream ss;

			if (aliveSeconds.count() <= 60)
			{
				ss << seconds << "s";
			}
			else
				ss << minutes << "m " << seconds << "s";

			m_timeAliveDisplay = ss.str();
			m_timeAlive = std::round(aliveSeconds.count() * 100.f) / 100.f;
			m_gStates = GameStates::gameOver;
			sceneManager();
		}

		// Updates timer for chaos
		m_timer.update(float(elapsed_seconds.count()), m_timerDuration, m_fromTimerColour, m_toTimerColour);
		
		// Primary AI Function
		if (currentChaos != ChaosEffects::switchingSides)
			ai(dt, accuracy);

		handleInput(dt);
		// update hud
		update();
		// draw shapes to screen
		draw();
	}
}
void GameEngine::mainMenu()
{
	float dt;
	
	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();

		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					m_window.close();
			}
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					handleButtons();
				}
			}
		}
		handleInput(dt);
		draw();
	}
}
void GameEngine::optionsMenu()
{
	float dt;
	m_title.setString("Options");

	bool sliderHeld = false;

	sf::Vector2f mousePos;

	std::stringstream ss;

	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();
		mousePos = sf::Vector2f(sf::Mouse::getPosition(m_window));

		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					m_gStates = GameStates::intro;
					sceneManager();
				}
			}
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					if (m_sliderLine.getGlobalBounds().findIntersection(sf::FloatRect(sf::Vector2f(mousePos), { 20,20 })))
						sliderHeld = true;

					handleButtons();
					
					// When Mouse Released Writes To JSON
					std::fstream fileWrite;
					fileWrite.open("./assets/json/Options.json", std::ios::out);
					optionsJS["volume"] = sf::Listener::getGlobalVolume();
					optionsJS["flashingLights"] = m_checkFlashingLights.isChecked();
					fileWrite << std::setw(4) << optionsJS;
					fileWrite.close();
				}
			}
			if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>())
			{
				if (mouseButtonReleased->button == sf::Mouse::Button::Left)
				{
					if (sliderHeld)
					{
						std::fstream fileWrite;
						fileWrite.open("./assets/json/Options.json", std::ios::out);
						optionsJS["volume"] = sf::Listener::getGlobalVolume(); // Sets Audio
						optionsJS["flashingLights"] = m_checkFlashingLights.isChecked();
						fileWrite << optionsJS;
						fileWrite.close();

						m_ballSound.play();
						sliderHeld = false;
					}
				}

			}
		}
		// Position of slider (Cant Go Too Far)
		if (sliderHeld)
		{
			if (!(mousePos.x < m_sliderLine.getPosition().x - m_sliderLine.getSize().x / 2) && !(mousePos.x > m_sliderLine.getPosition().x + m_sliderLine.getSize().x / 2))
				m_sliderBall.setPosition({ mousePos.x , m_sliderLine.getPosition().y });
			sf::Listener::setGlobalVolume((((m_sliderBall.getPosition().x) - (m_sliderLine.getPosition().x - (m_sliderLine.getSize().x / 2))) /
				((m_sliderLine.getPosition().x + (m_sliderLine.getSize().x / 2) - (m_sliderLine.getPosition().x - (m_sliderLine.getSize().x / 2))) / 100.f)));
		}
		handleInput(dt);
		draw();
	}
}

std::string GameEngine::textHandle()
{
	m_gStates = GameStates::textInput;

	m_bottomButton.setPosition({ m_bottomButton.getShape().getPosition().x, m_bottomButton.getShape().getPosition().y + 50 });
	m_bottomButton.setText("Enter");

	m_closeButton.setPosition({ m_closeButton.getShape().getPosition().x, float(m_window.getSize().y) - 75});

	float fontSize = float(m_textFieldText.getCharacterSize());
	float orFontSize = float(m_textFieldText.getCharacterSize());

	m_title.setString("Enter Your Name:");

	std::string text;
	while (m_window.isOpen())
	{
		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					m_gStates = GameStates::intro;
					sceneManager();
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Enter) {
					m_gStates = GameStates::playing;
					if (text.empty())
					{
						m_title.setString("Pong");
						return "name";
					}
					else
					{
						m_textFieldText.setString("");
						m_title.setString("Pong");
						return text;
					}
				}
				if (keyPressed->scancode == sf::Keyboard::Scancode::Backspace) {

					if (!text.empty())
					{
						text.pop_back();
						fontSize += 1.75; // Make Text Bigger
						m_textFieldText.setCharacterSize((unsigned int)(fontSize));
						if (fontSize >= orFontSize) // Clamps Font Size
						{
							fontSize = orFontSize;
							m_textFieldText.setCharacterSize((unsigned int)(fontSize));
						}
					}
					m_textFieldText.setString(text);
				}
			}
			if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
			{
				if ((textEntered->unicode < 128 && textEntered->unicode > 31) && !(textEntered->unicode == 32 && text.empty()))
				{
					text += char(textEntered->unicode);
					m_textFieldText.setString(text);
					if (text.length() > 16)
					{
						if (text.length() >= 20)
						{
							text.pop_back();
							m_textFieldText.setString(text);
							fontSize += 1.75;
							m_textFieldText.setCharacterSize((unsigned int)(fontSize));
						}
						fontSize -= 1.75; // Make Text Smaller To Fit More
						m_textFieldText.setCharacterSize((unsigned int)(fontSize));
					}
				}
			}
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					handleButtons();
					if (m_bottomButton.isClicked(m_window))
					{
						m_gStates = GameStates::playing;
						if (text.empty())
						{
							m_title.setString("Pong");
							return "name";
						}
						else
						{
							m_textFieldText.setString("");
							m_title.setString("Pong");
							return text;
						}
					}
				}
			}
		}
		draw();
	}
	return "";
}
void GameEngine::gameOverScreen()
{
	float dt = m_clock.restart().asSeconds();
	bool shouldLeave = false;
	
	while (m_window.isOpen())
	{
		dt = m_clock.restart().asSeconds();

		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					{
						m_gStates = GameStates::intro;
						sceneManager();
					}
				}
			}
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					handleButtons();
				}
			}
		}

		if (!m_hasGameEnded)
		{
			std::fstream fileWrite;
			std::fstream fileRead;

			// Different Modes different leaderboard
			switch (m_gMode)
			{
			case GameMode::endless:
			{
				if (std::filesystem::exists("./assets/json/LeaderboardEnd.json"))
				{
					fileRead.open("./assets/json/LeaderboardEnd.json", std::ios::in);

					json LeaderboardPar = json::parse(fileRead);

					fileRead.close();

					fileWrite.open("./assets/json/LeaderboardEnd.json", std::ios::out);

					// Rewrites all scores to new JSON
					for (auto& Item : LeaderboardPar.items())
					{
						if (m_boardName == Item.key() && int(LeaderboardPar.at(Item.key()).at("playerScore")) >= m_p1Score)
						{ 
							if (!(int(LeaderboardPar.at(Item.key()).at("playerScore")) == m_p1Score && double(LeaderboardPar.at(Item.key()).at("timeAlive")) > m_timeAlive))
							{
								shouldLeave = true;
							}
						}

						LeaderboardEnd[Item.key()]["playerScore"] = int(LeaderboardPar.at(Item.key()).at("playerScore"));
						LeaderboardEnd[Item.key()]["enemyScore"] = int(LeaderboardPar.at(Item.key()).at("enemyScore"));
						LeaderboardEnd[Item.key()]["timeAlive"] = double(LeaderboardPar.at(Item.key()).at("timeAlive"));
						LeaderboardEnd[Item.key()]["timeAliveDis"] = std::string(LeaderboardPar.at(Item.key()).at("timeAliveDis"));
						LeaderboardEnd[Item.key()]["minSec"] = std::string(LeaderboardPar.at(Item.key()).at("minSec"));
						LeaderboardEnd[Item.key()]["chaosFought"] = int(LeaderboardPar.at(Item.key()).at("chaosFought"));
						LeaderboardEnd[Item.key()]["date"] = std::string(LeaderboardPar.at(Item.key()).at("date"));
					}
				}
				else
				{
					fileWrite.open("./assets/json/LeaderboardEnd.json", std::ios::out);
				}

				// Writes new score to leaderboard
				if (!shouldLeave)
				{
					auto now = std::chrono::system_clock::now();
					std::time_t t_c = std::chrono::system_clock::to_time_t(now);
					LeaderboardEnd[m_boardName]["playerScore"] = m_p1Score;
					LeaderboardEnd[m_boardName]["enemyScore"] = m_p2Score;
					LeaderboardEnd[m_boardName]["timeAlive"] = m_timeAlive;
					LeaderboardEnd[m_boardName]["timeAliveDis"] = m_timeAliveDisplay;
					LeaderboardEnd[m_boardName]["minSec"] = m_minSec;
					LeaderboardEnd[m_boardName]["chaosFought"] = m_chaosStat;
					LeaderboardEnd[m_boardName]["date"] = std::string(std::ctime(&t_c));
				}

				fileWrite << std::setw(4) << LeaderboardEnd; //setw(4) makes 4 indent in JSON file for readability

				fileWrite.close();
				break;
			}
			case GameMode::timed:
			{
				if (std::filesystem::exists("./assets/json/LeaderboardTime.json"))
				{
					fileRead.open("./assets/json/LeaderboardTime.json", std::ios::in);

					json LeaderboardPar = json::parse(fileRead);

					fileRead.close();

					fileWrite.open("./assets/json/LeaderboardTime.json", std::ios::out);

					// Rewrites all scores to new JSON
					for (auto& Item : LeaderboardPar.items())
					{
						if (m_boardName == Item.key() && int(LeaderboardPar.at(Item.key()).at("playerScore")) >= m_p1Score)
						{
							if (!(int(LeaderboardPar.at(Item.key()).at("playerScore")) == m_p1Score && double(LeaderboardPar.at(Item.key()).at("timeAlive")) > m_timeAlive))
							{
								shouldLeave = true;
							}
						}

						LeaderboardTime[Item.key()]["playerScore"] = int(LeaderboardPar.at(Item.key()).at("playerScore"));
						LeaderboardTime[Item.key()]["enemyScore"] = int(LeaderboardPar.at(Item.key()).at("enemyScore"));
						LeaderboardTime[Item.key()]["timeAlive"] = double(LeaderboardPar.at(Item.key()).at("timeAlive"));
						LeaderboardTime[Item.key()]["timeAliveDis"] = std::string(LeaderboardPar.at(Item.key()).at("timeAliveDis"));
						LeaderboardTime[Item.key()]["minSec"] = std::string(LeaderboardPar.at(Item.key()).at("minSec"));
						LeaderboardTime[Item.key()]["chaosFought"] = int(LeaderboardPar.at(Item.key()).at("chaosFought"));
						LeaderboardTime[Item.key()]["date"] = std::string(LeaderboardPar.at(Item.key()).at("date"));
					}
				}
				else
				{
					fileWrite.open("./assets/json/LeaderboardTime.json", std::ios::out);
				}

				// Writes new score to leaderboard
				if (!shouldLeave)
				{
					auto now = std::chrono::system_clock::now();
					std::time_t t_c = std::chrono::system_clock::to_time_t(now);
					LeaderboardTime[m_boardName]["playerScore"] = m_p1Score;
					LeaderboardTime[m_boardName]["enemyScore"] = m_p2Score;
					LeaderboardTime[m_boardName]["timeAlive"] = m_timeAlive;
					LeaderboardTime[m_boardName]["timeAliveDis"] = m_timeAliveDisplay;
					LeaderboardTime[m_boardName]["minSec"] = m_minSec;
					LeaderboardTime[m_boardName]["chaosFought"] = m_chaosStat;
					LeaderboardTime[m_boardName]["date"] = std::string(std::ctime(&t_c));
				}

				fileWrite << std::setw(4) << LeaderboardTime; //setw(4) makes 4 indent in JSON file for readability

				fileWrite.close();

				break;
			}
			case GameMode::scored:
			{
				if (std::filesystem::exists("./assets/json/LeaderboardScore.json"))
				{
					fileRead.open("./assets/json/LeaderboardScore.json", std::ios::in);

					json LeaderboardPar = json::parse(fileRead);

					fileRead.close();

					fileWrite.open("./assets/json/LeaderboardScore.json", std::ios::out);

					// Rewrites all scores to new JSON
					for (auto& Item : LeaderboardPar.items())
					{
						if (m_boardName == Item.key() && (int(LeaderboardPar.at(Item.key()).at("playerScore")) >= m_p1Score))
						{
							if (!(int(LeaderboardPar.at(Item.key()).at("playerScore")) == m_p1Score && double(LeaderboardPar.at(Item.key()).at("timeAlive")) > m_timeAlive))
							{
								shouldLeave = true;
							}
						}

						LeaderboardScore[Item.key()]["playerScore"] = int(LeaderboardPar.at(Item.key()).at("playerScore"));
						LeaderboardScore[Item.key()]["enemyScore"] = int(LeaderboardPar.at(Item.key()).at("enemyScore"));
						LeaderboardScore[Item.key()]["timeAlive"] = double(LeaderboardPar.at(Item.key()).at("timeAlive"));
						LeaderboardScore[Item.key()]["timeAliveDis"] = std::string(LeaderboardPar.at(Item.key()).at("timeAliveDis"));
						LeaderboardScore[Item.key()]["minSec"] = std::string(LeaderboardPar.at(Item.key()).at("minSec"));
						LeaderboardScore[Item.key()]["chaosFought"] = int(LeaderboardPar.at(Item.key()).at("chaosFought"));
						LeaderboardScore[Item.key()]["date"] = std::string(LeaderboardPar.at(Item.key()).at("date"));
					}
				}
				else
				{
					fileWrite.open("./assets/json/LeaderboardScore.json", std::ios::out);
				}

				// Writes new score to leaderboard
				if (!shouldLeave)
				{
					auto now = std::chrono::system_clock::now();
					std::time_t t_c = std::chrono::system_clock::to_time_t(now);
					LeaderboardScore[m_boardName]["playerScore"] = m_p1Score;
					LeaderboardScore[m_boardName]["enemyScore"] = m_p2Score;
					LeaderboardScore[m_boardName]["timeAlive"] = m_timeAlive;
					LeaderboardScore[m_boardName]["timeAliveDis"] = m_timeAliveDisplay;
					LeaderboardScore[m_boardName]["minSec"] = m_minSec;
					LeaderboardScore[m_boardName]["chaosFought"] = m_chaosStat;
					LeaderboardScore[m_boardName]["date"] = std::string(std::ctime(&t_c));
				}

				fileWrite << std::setw(4) << LeaderboardScore; //setw(4) makes 4 indent in JSON file for readability

				fileWrite.close();
				break;
			}
			}

			// Checks Who wins
			if ((m_p1Score >= m_p2Score) && !m_hasGameEnded)
			{
				if (!m_stateBuffer.loadFromFile("./assets/audio/win.ogg"))
					return;
				m_stateSound.play();
				m_hasGameEnded = true;
				m_title.setString("You Win!");
			}
			else if ((m_p1Score < m_p2Score) && !m_hasGameEnded)
			{
				if (!m_stateBuffer.loadFromFile("./assets/audio/death.ogg"))
					return;
				m_stateSound.play();
				m_title.setString("You Lose :<");
				m_hasGameEnded = true;

			}

			std::stringstream ss;

			// Displays Stats
			ss << "Player Score: " << m_p1Score << std::endl << "Enemy Score: " << m_p2Score << std::endl << "Time Alive: " << m_timeAliveDisplay << std::endl << "Chaos Effects Fought: " << m_chaosStat;

			m_stats.setString(ss.str());

			handleInput(dt);
			draw();
		}
	}
}

void GameEngine::leaderboardScreen()
{
	m_closeButton.setPosition({ m_closeButton.getShape().getPosition().x, 60 });

	std::fstream fileRead;

	nameVec.clear();
	scoreVec.clear();

	m_title.setString("Leaderboard");

	// Different Modes different leaderboard
	switch (m_gMode)
	{
	case GameMode::endless:
	{
		if (std::filesystem::exists("./assets/json/LeaderboardEnd.json"))
		{
			fileRead.open("./assets/json/LeaderboardEnd.json", std::ios::in);
		}
		else
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}

		json LeaderboardPar = json::parse(fileRead);
		json LeaderboardCheck = LeaderboardPar;

		for (auto& Item : LeaderboardPar.items())
		{
			scoreVec.emplace_back(int(LeaderboardPar.at(Item.key()).at("playerScore")));
			nameVec.emplace_back("");
		}
			
		std::sort(scoreVec.begin(), scoreVec.end(), std::greater<>()); // Sorts Score

		for (auto& Item : LeaderboardPar.items())
		{
			for (int i = 0; i < nameVec.size(); i++)
			{
				if (scoreVec[i] == int(LeaderboardPar.at(Item.key()).at("playerScore")))
				{
					if ((std::find(nameVec.begin(), nameVec.end(), Item.key()) != nameVec.end()))
					{
						LeaderboardCheck.erase(Item.key());
						for (auto& Item2 : LeaderboardCheck.items())
						{
							for (int j = i; j < nameVec.size(); j++)
							{
								if (scoreVec[j] == int(LeaderboardCheck.at(Item.key()).at("playerScore")))
								{
									nameVec[j] = Item2.key(); // Allowing For 2 Of The Same Score At Once To Be Shown 
								}
							}
						}
					}
					else
						nameVec[i] = Item.key();
				}
			}
		}

		fileRead.close();

		scoreVec.resize(5);
		nameVec.resize(5);

		std::stringstream ss;
			
		// Sets Up Leaderboard Screen
		ss << "Name: " << nameVec[0] << "	Score: " << scoreVec[0];
		m_leaderPos1.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[1] << "	Score: " << scoreVec[1];
		m_leaderPos2.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[2] << "	Score: " << scoreVec[2];
		m_leaderPos3.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[3] << "	Score: " << scoreVec[3];
		m_leaderPos4.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[4] << "	Score: " << scoreVec[4];
		m_leaderPos5.setText(ss.str());
		ss.str("");
		break;
	}
	case GameMode::timed:
	{
		if (std::filesystem::exists("./assets/json/LeaderboardTime.json"))
		{
			fileRead.open("./assets/json/LeaderboardTime.json", std::ios::in);
		}
		else
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}

		json LeaderboardPar = json::parse(fileRead);
		json LeaderboardCheck = LeaderboardPar;

		for (auto& Item : LeaderboardPar.items())
		{
			scoreVec.emplace_back(int(LeaderboardPar.at(Item.key()).at("playerScore")));
			nameVec.emplace_back("");
		}

		std::sort(scoreVec.begin(), scoreVec.end(), std::greater<>());

		for (auto& Item : LeaderboardPar.items())
		{
			for (int i = 0; i < nameVec.size(); i++)
			{
				if (scoreVec[i] == int(LeaderboardPar.at(Item.key()).at("playerScore")))
				{
					if ((std::find(nameVec.begin(), nameVec.end(), Item.key()) != nameVec.end()))
					{
						LeaderboardCheck.erase(Item.key());
						for (auto& Item2 : LeaderboardCheck.items())
						{
							for (int j = i; j < nameVec.size(); j++)
							{
								if (scoreVec[j] == int(LeaderboardCheck.at(Item.key()).at("playerScore")))
								{
									nameVec[j] = Item2.key();
								}
							}
						}
					}
					else
						nameVec[i] = Item.key();
				}
			}
		}

		fileRead.close();

		scoreVec.resize(5);
		nameVec.resize(5);

		std::stringstream ss;

		ss << "Name: " << nameVec[0] << "	Score: " << scoreVec[0];
		m_leaderPos1.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[1] << "	Score: " << scoreVec[1];
		m_leaderPos2.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[2] << "	Score: " << scoreVec[2];
		m_leaderPos3.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[3] << "	Score: " << scoreVec[3];
		m_leaderPos4.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[4] << "	Score: " << scoreVec[4];
		m_leaderPos5.setText(ss.str());
		ss.str("");
		break;
	}
	case GameMode::scored:
	{
		if (std::filesystem::exists("./assets/json/LeaderboardScore.json"))
		{
			fileRead.open("./assets/json/LeaderboardScore.json", std::ios::in);
		}
		else
		{
			m_gStates = GameStates::intro;
			sceneManager();
		}

		json LeaderboardPar = json::parse(fileRead);
		json LeaderboardCheck = LeaderboardPar;

		for (auto& Item : LeaderboardPar.items())
		{
			scoreVec.emplace_back(int(LeaderboardPar.at(Item.key()).at("timeAlive")));
			nameVec.emplace_back("");
		}

		std::sort(scoreVec.begin(), scoreVec.end());

		for (auto& Item : LeaderboardPar.items())
		{
			for (int i = 0; i < nameVec.size(); i++)
			{
				if (scoreVec[i] == int(LeaderboardPar.at(Item.key()).at("timeAlive")))
				{
					if ((std::find(nameVec.begin(), nameVec.end(), Item.key()) != nameVec.end()))
					{
						LeaderboardCheck.erase(Item.key());
						for (auto& Item2 : LeaderboardCheck.items())
						{
							for (int j = i; j < nameVec.size(); j++)
							{
								if (scoreVec[j] == int(LeaderboardCheck.at(Item2.key()).at("timeAlive")))
								{
									nameVec[j] = Item2.key();
								}
							}
						}
					}
					else
						nameVec[i] = Item.key();
				}
			}
		}


		scoreVec.resize(5);
		nameVec.resize(5);

		std::stringstream ss;

		ss << "Name: " << nameVec[0] << "	Time: " << ((nameVec[0].empty()) ? "" : std::string(LeaderboardPar.at(nameVec[0]).at("timeAliveDis")));
		m_leaderPos1.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[1] << "	Time: " << ((nameVec[1].empty()) ? "" : std::string(LeaderboardPar.at(nameVec[1]).at("timeAliveDis")));
		m_leaderPos2.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[2] << "	Time: " << ((nameVec[2].empty()) ? "" : std::string(LeaderboardPar.at(nameVec[2]).at("timeAliveDis")));
		m_leaderPos3.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[3] << "	Time: " << ((nameVec[3].empty()) ? "" : std::string(LeaderboardPar.at(nameVec[3]).at("timeAliveDis")));
		m_leaderPos4.setText(ss.str());
		ss.str("");
		ss << "Name: " << nameVec[4] << "	Time: " << ((nameVec[4].empty()) ? "" : std::string(LeaderboardPar.at(nameVec[4]).at("timeAliveDis")));
		m_leaderPos5.setText(ss.str());
		ss.str("");
		fileRead.close();
		break;
	}
	}

	while (m_window.isOpen())
	{
		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					m_gStates = GameStates::intro;
					sceneManager();
				}
			}
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					handleButtons();
				}
			}
		}
		draw();
	}
}
void GameEngine::detailLeaderboardScreen(int buttonPressed)
{
	std::fstream fileRead;
	m_gStates = GameStates::detailedBoard;

	m_closeButton.setPosition({ m_closeButton.getShape().getPosition().x, m_closeButton.getShape().getPosition().y + 75 });

	// Prevents Clicking On Empty Name
	if (nameVec[buttonPressed - 1].empty())
	{
		m_gStates = GameStates::leaderboard;
		sceneManager();
		return;
	}
	
	m_title.setString(nameVec[buttonPressed - 1]);

	std::stringstream ss;

	// Different Modes Different Displayed Results
	switch (m_gMode)
	{
	case GameMode::endless:
	{
		fileRead.open("./assets/json/LeaderboardEnd.json", std::ios::in);
		json LeaderboardPar = json::parse(fileRead);

		ss << "Player Score: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("playerScore")) << std::endl
			<< "Enemy Score: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("enemyScore")) << std::endl
			<< "Time Alive: " << std::string(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("timeAliveDis")) << std::endl
			<< "Chaos Effects Fought: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("chaosFought")) << std::endl
			<< "Date: " << std::string(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("date"));
		break;
	}
	case GameMode::timed:
	{
		fileRead.open("./assets/json/LeaderboardTime.json", std::ios::in);
		json LeaderboardPar = json::parse(fileRead);

		ss << "Player Score: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("playerScore")) << std::endl
			<< "Enemy Score: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("enemyScore")) << std::endl
			<< "Chaos Effects Fought: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("chaosFought")) << std::endl
			<< "Date: " << std::string(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("date"));
		break;
	}
	case GameMode::scored:
	{
		fileRead.open("./assets/json/LeaderboardScore.json", std::ios::in);
		json LeaderboardPar = json::parse(fileRead);

		ss << "Player Score: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("playerScore")) << std::endl
			<< "Enemy Score: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("enemyScore")) << std::endl
			<< "Time Alive: " << std::string(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("timeAliveDis")) << std::endl
			<< "Chaos Effects Fought: " << int(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("chaosFought")) << std::endl
			<< "Date: " << std::string(LeaderboardPar.at(std::string(nameVec[buttonPressed - 1])).at("date"));
		break;
	}
	}

	
		
	m_detailsLeader.setString(ss.str());
	m_title.setCharacterSize(55);


	m_detailsLeader.setOrigin(m_detailsLeader.getLocalBounds().getCenter());
	m_detailsLeader.setPosition({ float(m_window.getSize().x) / 2, float(m_window.getSize().y) / 2 + 100});

	while (m_window.isOpen())
	{
		while (const std::optional event = m_window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				m_window.close();
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					m_gStates = GameStates::leaderboard;
					sceneManager();
				}
			}
			if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
			{
				if (mouseButtonPressed->button == sf::Mouse::Button::Left)
				{
					handleButtons();
				}
			}
		}
		draw();
	}
}