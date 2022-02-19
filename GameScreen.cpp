#include <SFML/Graphics.hpp>

#include <random>
#include <memory>

#include "GameScreen.h"
#include "GameOverScreen.h"
#include "Game.h"
#include "MenuScreen.h"
#include "number.h"

using namespace sfSnake;

GameScreen::GameScreen() : snake_()
{

}

void GameScreen::handleInput(sf::RenderWindow& window)
{
	snake_.handleInput(window);
}

void GameScreen::update(sf::Time delta)
{
	if (fruit_.size() <= 20)
		generateFruit();

	snake_.update(delta);
	snake_.checkFruitCollisions(fruit_);

	if (snake_.hitSelf())
		Game::Screen = std::make_shared<GameOverScreen>(snake_.getSize());
}

void GameScreen::render(sf::RenderWindow& window)
{
	switch (color)
	{
	case 1:
	{
		sf::RectangleShape line(sf::Vector2f(640, 480));
		line.rotate(0);
		line.setFillColor(sf::Color::White);
		line.setPosition(0, 0);
		window.draw(line);
		break;
	}
	case 2:
	{
		sf::RectangleShape line(sf::Vector2f(640, 480));
		line.rotate(0);
		line.setFillColor(sf::Color::Green);
		line.setPosition(0, 0);
		window.draw(line);
		break;
	}
	case 3:
	{
		sf::RectangleShape line(sf::Vector2f(640, 480));
		line.rotate(0);
		line.setFillColor(sf::Color::Magenta);
		line.setPosition(0, 0);
		window.draw(line);
		break;
	}
	}
	if (grid)
	{
		for (int i = 0; i < 25; i++)
		{
			sf::RectangleShape line(sf::Vector2f(640, 1));
			line.rotate(0);
			line.setFillColor(sf::Color::Black);
			line.setPosition(0, 20 * (i - 1));
			window.draw(line);
		}
		for (int i = 0; i < 33; i++)
		{
			sf::RectangleShape line(sf::Vector2f(640, 1));
			line.rotate(90);
			line.setFillColor(sf::Color::Black);
			line.setPosition(20 * (i - 1), 0);
			window.draw(line);
		}
	}
	snake_.render(window);
	for (auto fruit : fruit_)
		fruit.render(window);
}

void GameScreen::generateFruit()
{
	static std::default_random_engine engine;
	engine.seed(time(NULL));
	static std::uniform_int_distribution<int> xDistribution(0, Game::Width - SnakeNode::Width);
	static std::uniform_int_distribution<int> yDistribution(0, Game::Height - SnakeNode::Height);

	fruit_.push_back(Fruit(sf::Vector2f(xDistribution(engine), yDistribution(engine))));
}

