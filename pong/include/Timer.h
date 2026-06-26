#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "Chaos.h"
class Timer
{
private:
	sf::Vector2f m_size;
	sf::Vector2f m_barSize;
	float m_maxWidth;

	sf::RectangleShape m_backShape;
	sf::RectangleShape m_barShape;

	Chaos& m_chaos;

	sf::Color m_barColour;
	sf::Color m_barRed;

	sf::Font m_font;
	sf::Text m_text;

	// Lerp function
	float lerp(float a, float b, float t);

public:
	Timer(sf::Vector2f position, float width, float height, sf::Color m_backColour, Chaos& chaos);

	void draw(sf::RenderWindow& window);

	void update(float elapsedTime, int totalDuration, sf::Color fromColour, sf::Color toColour);
	void setTextColour(sf::Color colour);
};