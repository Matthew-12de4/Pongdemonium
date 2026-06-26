#include <iostream>
#include "GameEngine.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Pongdemonium", sf::Style::Close); // Creates Window

	GameEngine gameEngine(window);
	gameEngine.mainMenu(); // Starts On Main Menu
	return 0; 
}