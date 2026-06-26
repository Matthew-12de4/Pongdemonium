#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "Paddle.h"

class Ball
{
private:
	float m_speed;
	sf::CircleShape m_shape;
	sf::Vector2f m_velocity;
public:
	Ball(sf::Vector2f position, float radius, float speed, sf::Color color);
	void draw(sf::RenderWindow& window);
	int checkWin(sf::RenderWindow& window);
	void move(float dt, sf::RenderWindow& window);
	
	//Chaos Effect
	void jitterMove(float dt, sf::RenderWindow& window);
	
	// Hit Paddle + Overload
	bool hitPaddle(Paddle m_paddle, int paddleNum, sf::Vector2i offset);
	bool hitPaddle(Paddle m_paddle, int paddleNum);

	sf::Vector2f getPosition();
	void setPosition(float x, float y);

	void updateVelocity(float val);
	sf::CircleShape getShape();

	void setSpeed(float val);
	float getSpeed();

	void setSize(float radius);
	float getSize();

	void setColour(sf::Color colour);
};