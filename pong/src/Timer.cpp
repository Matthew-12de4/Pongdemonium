#include "Timer.h"

Timer::Timer(sf::Vector2f position, float width, float height, sf::Color m_backColour, Chaos& chaos)
	: m_size({width, height}), m_maxWidth(m_size.x - (m_size.x / 20)), m_chaos(chaos), m_text(m_font)
{

	if (!(m_font.openFromFile("./assets/fonts/digital-7.ttf")))
		return;

	m_backShape.setSize(m_size);
	m_backShape.setPosition(position);
	m_backShape.setFillColor(m_backColour);
	m_backShape.setOrigin(m_backShape.getSize() / 2.f);

	m_barSize = {0 , m_size.y - (m_size.x / 20) };


	m_barShape.setSize(m_barSize);
	m_barShape.setPosition(position);
	m_barShape.setFillColor(sf::Color::White);
	m_barShape.setOrigin({ (m_size.x - (m_size.x / 20)) / 2, m_barShape.getSize().y / 2.f });

	m_text.setCharacterSize(32);
	m_text.setFillColor(sf::Color::White);
	m_text.setLineAlignment(sf::Text::LineAlignment::Center);
	m_text.setPosition({ (m_backShape.getPosition().x + m_size.x /2) + 30, m_barShape.getPosition().y - (m_size.y / 2)});
	m_text.setString("0%");
}

float Timer::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void Timer::draw(sf::RenderWindow& window)
{
	window.draw(m_backShape);
	window.draw(m_barShape);
	window.draw(m_text);
}
void Timer::update(float elapsedTime, int totalDuration, sf::Color fromColour, sf::Color toColour)
{
	std::stringstream ss;

	float progress = elapsedTime / totalDuration;

	if (m_chaos.getChaos() == ChaosEffects::none)
	{
		m_barShape.setFillColor(sf::Color(
			uint8_t(lerp(toColour.r, fromColour.r, progress)),
			uint8_t(lerp(toColour.g, fromColour.g, progress)),
			uint8_t(lerp(toColour.b, fromColour.b, progress))
		));
	}
	else
	{
		m_barShape.setFillColor(sf::Color(
			uint8_t(lerp(fromColour.r, toColour.r, progress)),
			uint8_t(lerp(fromColour.g, toColour.g, progress)),
			uint8_t(lerp(fromColour.b, toColour.b, progress))
		));
	}


	m_chaos.setTextColour(m_barShape.getFillColor());

	 ss << int(progress * 100);
	m_text.setString(ss.str() + "%");

	m_barSize.x = progress * m_maxWidth;
	m_barShape.setSize({m_barSize.x, m_barSize.y});
}
void Timer::setTextColour(sf::Color colour)
{
	m_text.setFillColor(colour);
}