#include "Button.h"

Button::Button(sf::Vector2f position, float width, float height, sf::Color colour, std::string text, unsigned int charSize)
	: m_sprite(m_texture), m_text(font)
{
	if (!(font.openFromFile("./assets/fonts/arial_black.ttf")))
		return;
	m_size = { width, height };
	m_shape.setSize(m_size);
	m_shape.setPosition(position);
	m_shape.setFillColor(colour);
	m_shape.setOrigin(m_shape.getSize() / 2.f);

	m_text.setPosition(position);
	m_text.setString(text);
	m_text.setCharacterSize(charSize);
	m_text.setFillColor(sf::Color::White);
	m_text.setOrigin(m_text.getLocalBounds().getCenter());
}
// Overloaded Constructor To Allow Textures Or No Textures
Button::Button(sf::Vector2f position, std::string text, unsigned int charSize, std::string texturePath)
	: m_sprite(m_texture), m_text(font)
{
	if (!(font.openFromFile("./assets/fonts/arial_black.ttf")))
		return;
	
	if (!(m_texture.loadFromFile(texturePath)))
		return;
	
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(sf::IntRect({ 1, 0 }, sf::Vector2i(m_texture.getSize())));
	m_size = sf::Vector2f(m_texture.getSize());
	m_shape.setSize(m_size);
	m_shape.setPosition(position);
	m_shape.setFillColor(sf::Color::Transparent);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
	m_sprite.setPosition({ m_shape.getPosition().x - m_shape.getSize().x / 2, m_shape.getPosition().y - m_shape.getSize().y / 2});

	m_text.setPosition(position);
	m_text.setString(text);
	m_text.setCharacterSize(charSize);
	m_text.setFillColor(sf::Color::White);
	m_text.setOrigin(m_text.getLocalBounds().getCenter());
}
Button::Button(sf::Vector2f position, std::string uncheckedTexturePath, std::string checkedTexturePath, bool checked)
	: m_sprite(m_texture), m_text(font)
{
	if (!(font.openFromFile("./assets/fonts/arial_black.ttf")))
		return;
	
	if (!(m_texture.loadFromFile(uncheckedTexturePath)))
		return;
	if (!(m_texture2.loadFromFile(checkedTexturePath)))
		return;
	
	m_checked = checked;

	if (checked) m_sprite.setTexture(m_texture2);
	else m_sprite.setTexture(m_texture);

	m_sprite.setTextureRect(sf::IntRect({ 1, 0 }, (checked) ? sf::Vector2i(m_texture2.getSize()) : sf::Vector2i(m_texture.getSize())));
	m_size = sf::Vector2f((checked) ? m_texture2.getSize() : m_texture.getSize());

	m_shape.setSize(m_size);
	m_shape.setPosition(position);
	m_shape.setFillColor(sf::Color::Transparent);
	m_shape.setOrigin(m_shape.getSize() / 2.f);
	m_sprite.setPosition({ m_shape.getPosition().x - m_shape.getSize().x / 2, m_shape.getPosition().y - m_shape.getSize().y / 2});
}

void Button::setFont(sf::Font font)
{
	m_text.setFont(font);
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite, sf::BlendAlpha);
	window.draw(m_shape);
	window.draw(m_text);
}
sf::RectangleShape Button::getShape()
{
	return m_shape;
}

bool Button::isClicked(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);


	if (m_shape.getGlobalBounds().findIntersection(sf::FloatRect(sf::Vector2f(mousePos), { 1,1 })))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool Button::isClickedCheckbox(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);


	if (m_shape.getGlobalBounds().findIntersection(sf::FloatRect(sf::Vector2f(mousePos), { 1,1 })))
	{
		if (m_checked)
		{
			m_sprite.setTexture(m_texture);
			m_checked = false;
			return false;
		}
		else
		{
			m_sprite.setTexture(m_texture2);
			m_checked = true;
			return true;
		}
	}
	return m_checked;
}

bool Button::isChecked() { return m_checked; }

void Button::changeState(bool state)
{
	if (state)
	{
		m_checked = true;
		m_sprite.setTexture(m_texture2);
	}
	else
	{
		m_checked = false;
		m_sprite.setTexture(m_texture);
	}
}

void Button::setPosition(sf::Vector2f pos)
{
	m_shape.setPosition(pos);
	m_text.setPosition(pos);
	m_sprite.setPosition({ pos.x - m_texture.getSize().x / 2.f, pos.y - m_texture.getSize().y / 2.f });
}

void Button::setText(std::string text)
{
	m_text.setString(text);
	m_text.setOrigin(m_text.getLocalBounds().getCenter());
}
void Button::setCharacterSize(unsigned int charSize)
{
	m_text.setCharacterSize(charSize);
}