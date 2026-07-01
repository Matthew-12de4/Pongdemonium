#include "Chaos.h"
#include "Paddle.h"
#include "Ball.h"
#include <algorithm>

Chaos::Chaos(sf::RenderWindow& window, float& paddleSpeed, Paddle& paddle1, Paddle& paddle2, Ball& ball, std::vector<Ball>& ballVec)
	: m_window(window), m_ballVec(ballVec), m_text(m_font), m_PaddleSpeed(paddleSpeed), m_paddle1(paddle1), m_paddle2(paddle2), m_ball(ball)
{
	if (!(m_font.openFromFile("./assets/fonts/arial_black.ttf")))
		return;

	m_basePaddleSpeed = 0;
	m_baseBallSpeed = 0;
	m_cEffects = ChaosEffects::none;

	m_basePaddleSizeY = m_paddle1.getSize().y;
	m_baseBallSize = m_ball.getSize();
	m_basePaddle1Pos = m_paddle1.getShape().getPosition();
	m_basePaddle2Pos = m_paddle2.getShape().getPosition();

	m_text.setCharacterSize(32);
	m_text.setLineAlignment(sf::Text::LineAlignment::Center);
	m_text.setPosition(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) - 50 ));
	m_text.setString("Chaos Mode");
	m_text.setFillColor(sf::Color(250, 60, 250));
	
	m_pureChaos = false;

	m_metaEffects = {
		ChaosEffects::paddleSpeed, ChaosEffects::ballSpeed, ChaosEffects::paddleSize, ChaosEffects::ballSize, ChaosEffects::jitter, ChaosEffects::moveAnywhere,
		ChaosEffects::manyBalls, ChaosEffects::paddleColour, ChaosEffects::ballColour, ChaosEffects::rapidBackgroundColour, ChaosEffects::tripping, ChaosEffects::aiAimBot,
		ChaosEffects::cannotWin, ChaosEffects::invertColour, ChaosEffects::dvdWindow, ChaosEffects::rotatePaddle, ChaosEffects::laggy
	};
}

ChaosEffects Chaos::getChaos()
{
	return m_cEffects;
}

void Chaos::setChaos(ChaosEffects effect)
{
	m_cEffects = effect;
}

void Chaos::draw(sf::RenderWindow& window)
{
	window.draw(m_text);
}

void Chaos::setText(std::string text)
{
	m_text.setString(text);
}

void Chaos::setTextColour(sf::Color color)
{
	m_text.setFillColor(color);
}
std::string Chaos::toString(ChaosEffects effect)
{
	switch (effect)
	{
		case ChaosEffects::none: return "No Chaos";
		case ChaosEffects::paddleSpeed: return "Paddle Speed";
		case ChaosEffects::ballSpeed: return "Ball Speed";
		case ChaosEffects::paddleSize: return "Paddle Size";
		case ChaosEffects::ballSize: return "Ball Size";
		case ChaosEffects::jesusWheel: return "Jesus Take The Wheel";
		case ChaosEffects::jitter: return "Ball Jitter";
		case ChaosEffects::moveAnywhere: return "Move Anywhere < >";
		case ChaosEffects::manyBalls: return "Multiple Balls";
		case ChaosEffects::paddleColour: return "Paddle Colours";
		case ChaosEffects::ballColour: return "Ball Colours";
		case ChaosEffects::invisiblePaddle: return "Invisible Paddle";
		case ChaosEffects::invisibleBall: return "Invisible Ball";
		case ChaosEffects::rapidBackgroundColour: return "Rapid Background Change";
		case ChaosEffects::fakeCrash: return "Fake Crash";
		case ChaosEffects::tripping: return "Tripping";
		case ChaosEffects::aiAimBot: return "AI Aim Bot";
		case ChaosEffects::squareBall: return "Square Ball";
		case ChaosEffects::meta: return "Meta";
		case ChaosEffects::speedyTimer: return "Speedy Timer";
		case ChaosEffects::cannotWin: return "Cannot Win";
		case ChaosEffects::rickBall: return "Rick Ball";
		case ChaosEffects::invertControls: return "Inverted Controls";
		case ChaosEffects::switchingSides: return "Switch Sides";
		case ChaosEffects::dvdWindow: return "DVD Window";
		case ChaosEffects::meow: return "Meow";
		case ChaosEffects::rotatePaddle: return "Rotate Paddle";
		case ChaosEffects::invertColour: return "Invert Colours";
		case ChaosEffects::laggy: return "Lag";
		default: return "Error";
	}
}

void Chaos::setPureChaos(bool pureChaos)
{
	m_pureChaos = pureChaos;
}

bool Chaos::getPureChaos()
{
	return m_pureChaos;
}

void Chaos::setBasePaddleSpeed(float speed)
{
	m_basePaddleSpeed = speed;
}

void Chaos::setBaseBallSpeed(float speed)
{
	m_baseBallSpeed = speed;
}

void Chaos::selectChaos(float& aiFOV, bool flashingLights)
{
	//Random Number Generator
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 28); // full random

	//Resets All The Chaos Effects
	m_window.setFramerateLimit(0);
	m_metaSortedEffects.clear();
	m_ballVec.clear();
	m_PaddleSpeed = m_basePaddleSpeed;
	m_ball.setSpeed(m_baseBallSpeed);
	aiFOV = float(m_window.getSize().y / 2);
	m_paddle1.setSize({ m_paddle1.getSize().x, m_basePaddleSizeY });
	m_paddle2.setSize({ m_paddle2.getSize().x, m_basePaddleSizeY });
	m_ball.setSize(m_baseBallSize);
	m_paddle1.setPosition({ m_basePaddle1Pos.x, m_paddle1.getShape().getPosition().y});
	m_paddle2.setPosition({ m_basePaddle2Pos.x, m_paddle2.getShape().getPosition().y});
	m_paddle1.setColour(sf::Color::White);
	m_paddle2.setColour(sf::Color::White);
	m_ball.setColour(sf::Color::White);
	m_paddle1.setRotation(0);
	m_paddle2.setRotation(0);

	m_text.setFillColor(sf::Color::White);
	m_text.setFillColor(sf::Color(250, 60, 250));
	m_text.setPosition(sf::Vector2f(float(m_window.getSize().x) / 2, float(m_window.getSize().y) - 50 ));
	
	// Select Effect
	if (m_pureChaos)
	{
		m_cEffects = ChaosEffects(18); // Set effect to meta
	}
	else
	{
		int random = dist(gen);
		m_cEffects = ChaosEffects(random);
	}

	m_ss.str("");
	m_ss.clear();
	m_ss << toString(m_cEffects);
	m_text.setString(m_ss.str());
	
	switch (m_cEffects)
	{
	case ChaosEffects::paddleSpeed:
		paddleSpeed(aiFOV);
		break;
	case ChaosEffects::ballSpeed:
		ballSpeed(aiFOV);
		break;
	case ChaosEffects::paddleSize:
		paddleSize();
		break;
	case ChaosEffects::ballSize:
		ballSize();
		break;
	case ChaosEffects::manyBalls:
		manyBall();
		break;
	case ChaosEffects::invisiblePaddle:
		invisPaddle();
		break;
	case ChaosEffects::invisibleBall:
		invisBall();
		break;
	case ChaosEffects::rapidBackgroundColour:
		// Prevents Flashing Lights If Toggled
		if (!flashingLights)
			selectChaos(aiFOV, flashingLights);
		break;
	case ChaosEffects::meta:
		meta(aiFOV, flashingLights);
		break;
	case ChaosEffects::rotatePaddle:
		rotatePaddle();
		break;
	case ChaosEffects::laggy:
		laggy();
		break;
	default:
		break;
	}
}

void Chaos::paddleSpeed(float& aiFOV)
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 1);

	if (dist(gen))
	{
		m_PaddleSpeed = m_basePaddleSpeed * 5;
		aiFOV = float(m_window.getSize().y - 50);
	}
		
	else
	{
		m_PaddleSpeed = m_basePaddleSpeed / 2;
		aiFOV = float(m_window.getSize().y / 4);
	}
}

void Chaos::ballSpeed(float& aiFOV)
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, 1);

	int random = dist(gen);

	if (random)
	{
		m_ball.setSpeed(m_baseBallSpeed * 1.5f);
		aiFOV = float(m_window.getSize().y / 4);

	}
	else
	{
		m_ball.setSpeed(m_baseBallSpeed / 2);
		aiFOV = float(m_window.getSize().y - 50);

	}
}
void Chaos::paddleSize()
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distPN(0, 1);
	std::uniform_int_distribution<> dists(0, 90);
	int randomPN = distPN(gen);
	int randomSize = dists(gen);

	if (randomPN)
	{
		m_paddle1.setSize({ m_paddle1.getSize().x, m_paddle1.getSize().y - randomSize });
	}
	else
	{
		m_paddle1.setSize({ m_paddle1.getSize().x, m_paddle1.getSize().y + randomSize });
	}
	randomPN = distPN(gen);
	randomSize = dists(gen);
	if (randomPN)
	{
		m_paddle2.setSize({ m_paddle2.getSize().x, m_paddle2.getSize().y - randomSize });
	}
	else
	{
		m_paddle2.setSize({ m_paddle2.getSize().x, m_paddle2.getSize().y + randomSize });
	}
}
void Chaos::ballSize()
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dists(-7, 7);

	m_ball.setSize(m_ball.getSize() + dists(gen));

	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::manyBalls) > 0)
	{
		for (int i = 0; i < m_ballVec.size(); i++)
		{
			m_ballVec[i].setSize(m_ballVec[i].getSize() + dists(gen));
		}
	}

}
void Chaos::rotatePaddle()
{
	m_paddle1.setRotation(90);
	m_paddle1.setPosition({m_paddle1.getShape().getPosition().x + 50, m_paddle1.getShape().getPosition().y });
	m_paddle2.setRotation(90);
	m_paddle2.setPosition({m_paddle2.getShape().getPosition().x - 50, m_paddle2.getShape().getPosition().y });
}

void Chaos::manyBall()
{
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 50);
	std::uniform_int_distribution<> distY(-300, 300);
	std::uniform_int_distribution<> distS(-200, 200);

	for (int i = 0; i < dist(gen); i++)
	{
		m_ballVec.push_back(Ball({float(m_window.getSize().x) / 2,  (m_window.getSize().y / 2.f) + float(distY(gen)) }, m_baseBallSize, m_baseBallSpeed + distS(gen), sf::Color::White));
	}
}


void Chaos::invisPaddle()
{
	m_paddle1.setColour(sf::Color(0, 0, 0, 0));
	m_paddle2.setColour(sf::Color(0, 0, 0, 0));
}
void Chaos::invisBall()
{
	m_ball.setColour(sf::Color(0, 0, 0, 0));
}

void Chaos::meta(float& aiFOV, bool flashingLights)
{
	std::mt19937 gen(rd());
	std::vector<ChaosEffects> unsortedEffects = m_metaEffects;
	
	// Choose between 3 - 5 Effects
	std::uniform_int_distribution<> distAmount(3, 8);

	if (!flashingLights)
		unsortedEffects.erase(unsortedEffects.begin() + 9);

	for (int i = 0; i < distAmount(gen); i++)
	{
		std::uniform_int_distribution<> dist(1, int(unsortedEffects.size()) - 1);

		int rand = dist(gen);
		m_metaSortedEffects.push_back(unsortedEffects[rand]);
		unsortedEffects.erase(unsortedEffects.begin() + rand);
	}
	
	// Displays all current active Chaos Effects
	float moveOffset = m_window.getSize().y - m_text.getGlobalBounds().getCenter().y;
	m_ss.str("");
	m_ss.clear();
	m_metaSortedEffects.clear();
	m_metaSortedEffects = {ChaosEffects::paddleColour, ChaosEffects::ballSize, ChaosEffects::manyBalls, ChaosEffects::dvdWindow, ChaosEffects::jitter};
	int j = 0;
	for (auto & i : m_metaSortedEffects)
	{
		j++;
		m_ss << toString(i) << "\n";
		//m_text.move({0, -moveOffset});
	}
	std::cerr << j << std::endl;
	m_text.setPosition({float(m_window.getSize().x) / 2, (float(m_window.getSize().y) - 50) - moveOffset * (j + 1) });
	
	m_text.setString(m_ss.str());

	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::manyBalls) > 0)
		manyBall();
	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::paddleSpeed) > 0)
		paddleSpeed(aiFOV);
	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::ballSpeed) > 0)
		ballSpeed(aiFOV);
	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::paddleSize) > 0)
		paddleSize();
	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::ballSize) > 0)
		ballSize();
	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::rotatePaddle) > 0)
		rotatePaddle();
	if (std::count(m_metaSortedEffects.begin(), m_metaSortedEffects.end(), ChaosEffects::laggy) > 0)
		laggy();
}
std::vector<ChaosEffects> Chaos::getMetaEffects()
{
	return !m_metaSortedEffects.empty() ? m_metaSortedEffects : m_metaEffects;
}

void Chaos::laggy()
{
	m_window.setFramerateLimit(15);
}