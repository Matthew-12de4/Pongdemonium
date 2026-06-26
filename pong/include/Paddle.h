#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Paddle
{
private:
	float m_speed = 500.f;
	sf::Vector2f m_size;
	sf::RectangleShape m_shape;

public:
	Paddle(sf::Vector2f position, float width, float height, sf::Color color);
	void draw(sf::RenderWindow& window);
	
	// Move Functions
	void move(float dt, float windowYVal);
	void moveLat(float dt, float windowXVal);

	sf::FloatRect getBounds() const;
	sf::RectangleShape getShape();
	
	float getSpeed();
	void setSpeed(float speed);
	
	void setPosition(sf::Vector2f pos);
	
	sf::Vector2f getSize();
	void setSize(sf::Vector2f size);
	
	void setRotation(float rotation);

	void setColour(sf::Color colour);

};