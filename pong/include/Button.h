#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
private:
	sf::Vector2f m_size;
	sf::RectangleShape m_shape;

	sf::Texture m_texture;
	sf::Texture m_texture2;
	sf::Sprite m_sprite;

	sf::Font font;
	sf::Text m_text;

	bool m_checked = false;;

public:
	Button(sf::Vector2f position, float width, float height, sf::Color colour, std::string text, unsigned int charSize); // coloured buttons
	Button(sf::Vector2f position, std::string text, unsigned int charSize, std::string texturePath); // textured buttons
	Button(sf::Vector2f position, std::string uncheckedTexturePath, std::string checkedTexturePath, bool checked); // Checkbox

	void setFont(sf::Font font);

	void draw(sf::RenderWindow& window);
	sf::RectangleShape getShape();

	bool isClicked(sf::RenderWindow& window);
	bool isClickedCheckbox(sf::RenderWindow& window);
	bool isChecked();
	void changeState(bool state);

	void setPosition(sf::Vector2f pos);

	void setText(std::string text);
	void setCharacterSize(unsigned int charSize);
};