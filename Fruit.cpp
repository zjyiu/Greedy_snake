#include <SFML/Graphics.hpp>

#include "Fruit.h"
#include <stdlib.h>
#include <time.h> 

using namespace sfSnake;

const float Fruit::Radius = 5.f;

Fruit::Fruit(sf::Vector2f position)
{
	shape_.setPosition(position);
	shape_.setRadius(Fruit::Radius);
	srand((unsigned)time(NULL));
	color_ = (rand() % 4) + 1;
	if (color_ == 1)
		color_ = (rand() % 2) + 4;
	else
		color_ = (rand() % 3) + 1;
	switch (color_)
	{
	case 1:
		shape_.setFillColor(sf::Color::Red);
		break;
	case 2:
		shape_.setFillColor(sf::Color::Blue);
		break;
	case 3:
		shape_.setFillColor(sf::Color::Cyan);
		break;
	case 4:
		shape_.setFillColor(sf::Color::Yellow);
		break;
	case 5:
		shape_.setFillColor(sf::Color::Black);
		break;
	}
}

void Fruit::render(sf::RenderWindow& window)
{
		window.draw(shape_);
}

sf::FloatRect Fruit::getBounds() const
{
	return shape_.getGlobalBounds();
}