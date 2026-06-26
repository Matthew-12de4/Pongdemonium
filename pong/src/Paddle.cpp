#include "Paddle.h"

Paddle::Paddle(sf::Vector2f position, float width, float height, sf::Color color)
{
	m_size.x = width;
	m_size.y = height;
	m_shape.setSize(m_size);
	m_shape.setPosition(position);
	m_shape.setFillColor(color);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}

void Paddle::draw(sf::RenderWindow& window)
{
	window.draw(m_shape);
}

void Paddle::move(float dt, float windowYVal)
{
	if (m_shape.getPosition().y - m_shape.getSize().y / 2 > windowYVal) //down
		m_shape.move({ 0, -m_speed * dt });

	if (m_shape.getPosition().y + m_shape.getSize().y / 2 < windowYVal) //up
		m_shape.move({ 0, m_speed * dt });
}
void Paddle::moveLat(float dt, float windowXVal)
{
	if (m_shape.getPosition().x - m_shape.getSize().x / 2 > windowXVal) //right
		m_shape.move({ -m_speed * dt, 0 });

	if (m_shape.getPosition().x + m_shape.getSize().x / 2 < windowXVal) //left
		m_shape.move({ m_speed * dt, 0 });
}

sf::FloatRect Paddle::getBounds() const
{
	return m_shape.getGlobalBounds();
}
sf::RectangleShape Paddle::getShape()
{
	return m_shape;
}
float Paddle::getSpeed()
{
	return m_speed;
}
sf::Vector2f Paddle::getSize()
{
	return m_shape.getSize();
}

void Paddle::setSpeed(float speed)
{
	m_speed = speed;
}
void Paddle::setPosition(sf::Vector2f pos)
{
	m_shape.setPosition(pos);
}
void Paddle::setSize(sf::Vector2f size)
{
	m_shape.setSize(size);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
}
void Paddle::setRotation(float rotation)
{
	m_shape.setRotation(sf::degrees(rotation));
}

void Paddle::setColour(sf::Color colour)
{
	m_shape.setFillColor(colour);
}

