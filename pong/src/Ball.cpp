#include "Ball.h"

Ball::Ball(sf::Vector2f position, float radius, float speed, sf::Color color)
{
	m_speed = speed;
	m_velocity.x = speed;
	m_velocity.y = speed;
	m_shape.setRadius(radius);
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin({ m_shape.getRadius() / 2, m_shape.getRadius() / 2 });
}

void Ball::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
}
int Ball::checkWin(sf::RenderWindow& window)
{
	if (m_shape.getPosition().x < 1)
		return 1;
	else if (m_shape.getPosition().x + m_shape.getRadius() * 2 > window.getSize().x - 1)
		return 2;

	return 0;
}

void Ball::move(float dt, sf::RenderWindow& window)
{
	m_shape.move(m_velocity * dt);
	if (m_shape.getPosition().y <= 0 ||
		m_shape.getPosition().y + m_shape.getRadius() * 2 >= window.getSize().y)
		m_velocity.y = -m_velocity.y;
	if (m_shape.getPosition().y <= -1)
	{
		m_shape.setPosition({ m_shape.getPosition().x, 2 });
	}
	else if (m_shape.getPosition().y + m_shape.getRadius() * 2 >= window.getSize().y + 1)
	{
		m_shape.setPosition({ m_shape.getPosition().x, window.getSize().y - (m_shape.getRadius() * 2) + 1 });
	}
}

void Ball::jitterMove(float dt, sf::RenderWindow& window)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(-1, 1);

	// Adds Random Number To Position
	m_shape.move((m_velocity * dt) + sf::Vector2f(dist(gen), dist(gen)));
	if (m_shape.getPosition().y <= 0 ||
		m_shape.getPosition().y + m_shape.getRadius() * 2 >= window.getSize().y)
		m_velocity.y = -m_velocity.y;
	if (m_shape.getPosition().y <= -1)
	{
		m_shape.setPosition({ m_shape.getPosition().x, 2 });
	}
	else if (m_shape.getPosition().y + m_shape.getRadius() * 2 >= window.getSize().y + 1)
	{
		m_shape.setPosition({ m_shape.getPosition().x, window.getSize().y - (m_shape.getRadius() * 2) + 1});
	}
}
bool Ball::hitPaddle(Paddle m_paddle, int paddleNum, sf::Vector2i offset) // Overloaded To Add Offset
{
	sf::Vector2f paddlePos = m_paddle.getShape().getPosition();
	sf::Vector2f paddleSize = m_paddle.getShape().getSize();
	sf::Vector2f ballPos = m_shape.getPosition();
	float ballSize = m_shape.getRadius();
	switch (paddleNum)
	{
	case 1: // Paddle 1
		if (ballPos.x - ballSize <= paddlePos.x + paddleSize.x/2 + offset.x && ballPos.y + ballSize >= paddlePos.y - paddleSize.y / 2 + offset.y && ballPos.y - ballSize <= paddlePos.y + paddleSize.y / 2 - offset.y)
			return true;
		else
			return false;
		break;
	case 2: // Paddle 2
		if (ballPos.x + ballSize >= paddlePos.x - paddleSize.x/2 - offset.x && ballPos.y + ballSize >= paddlePos.y - paddleSize.y / 2 + offset.y && ballPos.y - ballSize <= paddlePos.y + paddleSize.y / 2 - offset.y)
			return true;
		else
			return false;
		break;
	default:
		return false;
		break;
	}
	
}
bool Ball::hitPaddle(Paddle m_paddle, int paddleNum)
{
	sf::Vector2f paddlePos = m_paddle.getShape().getPosition();
	sf::Vector2f paddleSize = m_paddle.getShape().getSize();
	sf::Vector2f ballPos = m_shape.getPosition();
	float ballSize = m_shape.getRadius();
	switch (paddleNum)
	{
	case 1:
		if ((ballPos.x - ballSize <= paddlePos.x + paddleSize.x/2) && ((ballPos.y + ballSize >= paddlePos.y - paddleSize.y / 2) && (ballPos.y - ballSize <= paddlePos.y + paddleSize.y / 2)))
			return true;
		else
			return false;
		break;
	case 2:
		if ((ballPos.x + ballSize >= paddlePos.x - paddleSize.x/2) && ((ballPos.y + ballSize >= paddlePos.y - paddleSize.y / 2) && (ballPos.y - ballSize <= paddlePos.y + paddleSize.y / 2)))
			return true;
		else
			return false;
		break;
	default:
		return false;
		break;
	}
	
}

sf::Vector2f Ball::getPosition()
{
	return m_shape.getPosition();
}

void Ball::setPosition(float x, float y)
{
	m_shape.setPosition({ x, y });
}

void Ball::updateVelocity(float val)
{
	m_velocity.x = m_speed * val;
}

sf::CircleShape Ball::getShape()
{
	return m_shape;
}

void Ball::setSpeed(float val)
{
	m_speed = val;

	// Continues In The Correct Direction
	if (m_velocity.x < 0)
	{
		m_velocity.x = -m_speed;
	}
	else
	{
		m_velocity.x = m_speed;
	}
	if (m_velocity.y < 0)
	{
		m_velocity.y = -m_speed;
	}
	else
	{
		m_velocity.y = m_speed;
	}
}

float Ball::getSpeed()
{
	return m_speed;
}

void Ball::setSize(float radius)
{
	m_shape.setRadius(radius);
	m_shape.setOrigin({ m_shape.getRadius() / 2, m_shape.getRadius() / 2 });
}
float Ball::getSize()
{
	return m_shape.getRadius();
}

void Ball::setColour(sf::Color colour)
{
	m_shape.setFillColor(colour);
}