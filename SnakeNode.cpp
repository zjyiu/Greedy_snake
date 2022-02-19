#include <SFML/Graphics.hpp>

#include "SnakeNode.h"

using namespace sfSnake;

const float SnakeNode::Width = 10.f;
const float SnakeNode::Height = 10.f;

SnakeNode::SnakeNode(sf::Vector2f position)
	: position_(position)
{
	shape_.setPosition((position.x+5.f),position.y);
	shape_.setFillColor(sf::Color::Black);
	shape_.setSize(sf::Vector2f(SnakeNode::Width*2, SnakeNode::Height/2));
	c_shape_.setPosition(position_);
	c_shape_.setFillColor(sf::Color::Yellow);
	c_shape_.setRadius(10.f);
}

void SnakeNode::setPosition(sf::Vector2f position)
{
	position_ = position;
	shape_.setPosition(position_);
	c_shape_.setPosition(position_);
}

void SnakeNode::setPosition(float x, float y)
{
	position_.x = x;
	position_.y = y;
	shape_.setPosition(position_);
	c_shape_.setPosition(position_);
}

void SnakeNode::move(float xOffset, float yOffset)
{
	position_.x += xOffset;
	position_.y += yOffset;
	shape_.setPosition(position_);
	c_shape_.setPosition(position_);
}

sf::FloatRect SnakeNode::getBounds() const
{
	return c_shape_.getGlobalBounds();
}

sf::Vector2f SnakeNode::getPosition() const
{
	return position_;
}

void SnakeNode::render(sf::RenderWindow& window)
{
	window.draw(c_shape_);
	window.draw(shape_);
}
