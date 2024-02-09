#pragma once

#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Assets
{
	std::map<std::string, sf::Texture> m_textures;
	std::map<std::string, Animation> m_animations;
	std::map<std::string, sf::Sound> m_sounds;
	std::map<std::string, sf::Font> m_fonts;

public:
	Assets() {}

	void addTexture(std::string& name, std::string& path);
	void addAnimation(std::string& animationName, std::string& textureName, int frameCount, int animSpeed);
		//void addAnimation(std::string& name, Animation animation);
	void addSound(std::string& name, std::string& path);
	void addFont(std::string& name, std::string& path);

	const sf::Texture& getTexture(std::string& name) const;
	const Animation& getAnimation(const std::string& name) const;
	const sf::Sound& getSound(std::string& name) const;
	const sf::Font& getFont(std::string& name) const;
	const sf::Font& getFont(const std::string& name) const;
};