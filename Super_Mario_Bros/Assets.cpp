#include <iostream>
#include <cassert>

#include "Assets.h"

void Assets::addTexture(std::string& name, std::string& path)
{
	sf::Texture texture;
	if (!texture.loadFromFile(path))
	{
		std::cout << "Could not load texture! File: " << path << "\n";
		exit(-1);
	}
	texture.setSmooth(true);
	std::cout << "Texture Name: " << name << ", path: " << path << "\n";
	m_textures[name] = texture;
}

void Assets::addAnimation(std::string& animationName, std::string& textureName, int frameCount, int animSpeed)
{
	std::cout << "Animation name: " << animationName << ", textureName: " << textureName << ", frameCount: " << frameCount << ", animSpeed: " << animSpeed << "\n";
	Animation anim(animationName, getTexture(textureName), frameCount, animSpeed);
	m_animations[animationName] = anim;
}

void Assets::addSound(std::string& name, std::string& path)
{
	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile(path))
	{
		std::cout << "Could not load sound! File: " << path << "\n";
		exit(-1);
	}

	sf::Sound sound;
	sound.setBuffer(buffer);
	
	m_sounds[name] = sound;
}

void Assets::addFont(std::string& name, std::string& path)
{
	sf::Font font;
	if (!font.loadFromFile(path))
	{
		std::cout << "Could not load font! File: " << path << "\n";
		exit(-1);
	}

	m_fonts[name] = font;
}

const sf::Texture& Assets::getTexture(std::string& name) const
{
	assert(m_textures.find(name) != m_textures.end());
	return m_textures.at(name);

}

const Animation& Assets::getAnimation(const std::string& name) const
{
	assert(m_animations.find(name) != m_animations.end());
	return m_animations.at(name);
}

const sf::Sound& Assets::getSound(std::string& name) const
{
	assert(m_sounds.find(name) != m_sounds.end());
	return m_sounds.at(name);
}

const sf::Font& Assets::getFont(std::string& name) const
{
	assert(m_fonts.find(name) != m_fonts.end());
	return m_fonts.at(name);
}

// const std::string so that we can do getFont("Const string")
const sf::Font& Assets::getFont(const std::string& name) const
{
	assert(m_fonts.find(name) != m_fonts.end());
	return m_fonts.at(name);
}