#include "Animation.h"
#include <iostream>
#include <cmath>


Animation::Animation(const std::string & name, const sf::Texture &t)
	: m_name(name)
{
	m_size = Vec2((float)t.getSize().x / 144, (float)t.getSize().y);
	m_sprite.setTexture(t);
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

Animation::Animation(const std::string & name, const sf::Texture& t, size_t frameCount, size_t speed) 
	: m_name(name)
	, m_frameCount(frameCount)
	, m_speed(speed)
{
	m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
	m_sprite.setTexture(t);
	m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
	m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update()
{
	// add the speed variable to the current frame
	m_currentFrame++;

	int animationFrame = (int)(m_currentFrame / m_speed) % (int)m_frameCount;
	getSprite().setTextureRect(sf::IntRect(animationFrame * m_size.x, 0, m_size.x, m_size.y));
}

const Vec2& Animation::getSize() const
{
	return m_size;
}

const std::string& Animation::getName() const
{
	return m_name;
}

sf::Sprite& Animation::getSprite()
{
	return m_sprite;
}

bool Animation::hasEnded() const
{
	if ((int)(m_currentFrame / m_speed) % (int)m_frameCount == m_frameCount - 1)
		return true;

	return false;
}

size_t Animation::getFrameCount()
{
	return m_frameCount;
}
