#pragma once

#include <random>
#include <iostream>
#include <sstream>
#include "Paddle.h"
#include "Ball.h"

enum class ChaosEffects { // 28 Effects
	none,
	paddleSpeed, ballSpeed, paddleSize, ballSize, jesusWheel, jitter, moveAnywhere, manyBalls,
	paddleColour, ballColour, invisiblePaddle, invisibleBall, rapidBackgroundColour,
	fakeCrash, tripping, aiAimBot, squareBall, meta, speedyTimer, cannotWin, rickBall,
	invertControls, switchingSides, dvdWindow, meow, rotatePaddle, invertColour, laggy
};

class Chaos
{
private:
	sf::RenderWindow& m_window;
	
	ChaosEffects m_cEffects = ChaosEffects::none;

	// Vectors
	std::vector<Ball>& m_ballVec;
	std::vector<ChaosEffects> m_metaEffects;
	std::vector<ChaosEffects> m_metaSortedEffects;
	std::vector<sf::Color> m_colours;

	// Text
	sf::Font m_font;
	sf::Text m_text;

	// Paddle/Ball
	float& m_PaddleSpeed;
	float m_basePaddleSpeed = 0;
	float m_baseBallSpeed = 0;
	float m_basePaddleSizeY = 0;
	float m_baseBallSize = 0;
	sf::Vector2f m_basePaddle1Pos;
	sf::Vector2f m_basePaddle2Pos;

	Paddle& m_paddle1;
	Paddle& m_paddle2;
	Ball& m_ball;

	// Required for random
	std::random_device rd;
public:
	Chaos(sf::RenderWindow& window, float& paddleSpeed, Paddle& paddle1, Paddle& paddle2, Ball& ball, std::vector<Ball>& ballVec);
	
	// Get and Set for Chaos
	ChaosEffects getChaos();
	void setChaos(ChaosEffects effect);

	void draw(sf::RenderWindow& window);

	// Chaos Effects
	void selectChaos(float& aiFOV, bool flashingLights);
	
	void setText(std::string text);
	void setTextColour(sf::Color color);

	void setBasePaddleSpeed(float speed);
	void setBaseBallSpeed(float speed);

	void paddleSpeed(float& aiFOV);
	void ballSpeed(float& aiFOV);

	void paddleSize();
	void ballSize();
	
	void rotatePaddle();

	void manyBall();

	void invisPaddle();
	void invisBall();

	void meta(float& aiFOV, bool flashingLights);
	std::vector<ChaosEffects> getMetaEffects();
	
	void laggy();
};