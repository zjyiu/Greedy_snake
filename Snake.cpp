#include <SFML/Graphics.hpp>

#include <memory>
#include <iostream>
#include <math.h>

#include "Snake.h"
#include "Game.h"
#include "Fruit.h"
#include "SnakeNode.h"
#include "GameOverScreen.h"

using namespace sfSnake;

const int Snake::InitialSize = 5;

Snake::Snake() : direction_(Direction::Up), hitSelf_(false)
{
	initNodes();

	pickupBuffer_.loadFromFile("Sounds/pickup.aiff");
	pickupSound_.setBuffer(pickupBuffer_);
	pickupSound_.setVolume(30);

	dieBuffer_.loadFromFile("Sounds/die.wav");
	dieSound_.setBuffer(dieBuffer_);
	dieSound_.setVolume(50);
}

void Snake::initNodes()
{
	texture.loadFromFile("2.jpg");
	head.setTexture(texture);
	head.setTextureRect(sf::IntRect(500, 500, 20, 20));
	head.setPosition(Game::Width / 2 - SnakeNode::Width / 2, Game::Height / 2 - (SnakeNode::Height / 2) - 20.f);
	for (int i = 0; i < Snake::InitialSize; ++i)
	{
		nodes_.push_back(SnakeNode(sf::Vector2f(
			Game::Width / 2 - SnakeNode::Width / 2,
			Game::Height / 2 - (SnakeNode::Height / 2) + (i * 20.f))));
	}
}

void Snake::handleInput(sf::RenderWindow& window)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		direction_ = Direction::Up;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		direction_ = Direction::Down;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		direction_ = Direction::Left;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		direction_ = Direction::Right;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		MousePosition = sf::Mouse::getPosition(window);
		direction_ = Direction::Mouse;
		sf::Vector2f SnakePosition = nodes_[0].getPosition();
		float dx = MousePosition.x - SnakePosition.x;
		float dy = MousePosition.y - SnakePosition.y;
		Mouse_x = dx / pow((dx * dx + dy * dy), 0.5);
		Mouse_y = dy / pow((dx * dx + dy * dy), 0.5);
	}
}

void Snake::update(sf::Time delta)
{
	move();
	checkEdgeCollisions();
	checkSelfCollisions();
}

void Snake::checkFruitCollisions(std::vector<Fruit>& fruits)
{
	decltype(fruits.begin()) toRemove = fruits.end();


	for (auto it = fruits.begin(); it != fruits.end(); ++it)
	{
		if (it->getBounds().intersects(head.getGlobalBounds()))
			toRemove = it;
	}

	if (toRemove != fruits.end())
	{
		pickupSound_.play();
		switch (toRemove->color_)
		{
		case 1:
			grow();
			grow();
			grow();
			break;
		case 2:
			grow();
			grow();
			break;
		case 3:
			grow();
			break;
		}
		fruits.erase(toRemove);
	}
}

void Snake::grow()
{
	switch (direction_)
	{
	case Direction::Up:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
			nodes_[nodes_.size() - 1].getPosition().y + SnakeNode::Height)));
		break;
	case Direction::Down:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x,
			nodes_[nodes_.size() - 1].getPosition().y - SnakeNode::Height)));
		break;
	case Direction::Left:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x + SnakeNode::Width,
			nodes_[nodes_.size() - 1].getPosition().y)));
		break;
	case Direction::Right:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x - SnakeNode::Width,
			nodes_[nodes_.size() - 1].getPosition().y)));
		break;
	case Direction::Mouse:
		nodes_.push_back(SnakeNode(sf::Vector2f(nodes_[nodes_.size() - 1].getPosition().x - Mouse_x * SnakeNode::Width,
			nodes_[nodes_.size() - 1].getPosition().y - Mouse_y * SnakeNode::Height)));
		break;
	}
}

unsigned Snake::getSize() const
{
	return nodes_.size();
}

bool Snake::hitSelf() const
{
	return hitSelf_;
}

void Snake::checkSelfCollisions()
{
	SnakeNode& headNode = nodes_[0];

	for (decltype(nodes_.size()) i = 1; i < nodes_.size(); ++i)
	{
		if (head.getGlobalBounds().intersects(nodes_[i].getBounds()) && i != 1 && i != 2)
		{
			dieSound_.play();
			sf::sleep(sf::seconds(dieBuffer_.getDuration().asSeconds()));
			hitSelf_ = true;
		}
	}
}

void Snake::checkEdgeCollisions()
{
	if (head.getPosition().x <= 0)
		head.setPosition(Game::Width, head.getPosition().y);
	else if (head.getPosition().x >= Game::Width)
		head.setPosition(0, head.getPosition().y);
	else if (head.getPosition().y <= 0)
		head.setPosition(head.getPosition().x, Game::Height);
	else if (head.getPosition().y >= Game::Height)
		head.setPosition(head.getPosition().x, 0);

	SnakeNode& headNode = nodes_[0];

	if (headNode.getPosition().x <= 0)
		headNode.setPosition(Game::Width, headNode.getPosition().y);
	else if (headNode.getPosition().x >= Game::Width)
		headNode.setPosition(0, headNode.getPosition().y);
	else if (headNode.getPosition().y <= 0)
		headNode.setPosition(headNode.getPosition().x, Game::Height);
	else if (headNode.getPosition().y >= Game::Height)
		headNode.setPosition(headNode.getPosition().x, 0);
}

void Snake::move()
{
	for (decltype(nodes_.size()) i = nodes_.size() - 1; i > 0; --i)
	{
		nodes_[i].setPosition(nodes_.at(i - 1).getPosition());
	}
	
	nodes_[0].setPosition(head.getPosition());

	sf::Vector2f HeadPosition = head.getPosition();
	switch (direction_)
	{
	case Direction::Up:
		head.setPosition(HeadPosition.x, HeadPosition.y - SnakeNode::Height * 2);
		break;
	case Direction::Down:
		head.setPosition(HeadPosition.x, HeadPosition.y + SnakeNode::Height * 2);
		break;
	case Direction::Left:
		head.setPosition(HeadPosition.x - SnakeNode::Width * 2, HeadPosition.y);
		break;
	case Direction::Right:
		head.setPosition(HeadPosition.x + SnakeNode::Width * 2, HeadPosition.y);
		break;
	case Direction::Mouse:
		head.setPosition(HeadPosition.x + Mouse_x * SnakeNode::Width * 2, HeadPosition.y + Mouse_y * SnakeNode::Width * 2);
		break;
	}
}

void Snake::render(sf::RenderWindow& window)
{
	window.draw(head);
	for (auto node : nodes_)
		node.render(window);
}

