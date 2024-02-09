#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Play.h"
#include "Scene_Menu.h"

#include <iostream>
#include <fstream>

GameEngine::GameEngine(const std::string& path)
{
	init(path);
}

void GameEngine::update()
{
	// update the scene ? or assets ?
	sUserInput();
	//std::cout << m_currentScene << ", the current scene.\n";
	currentScene()->update();
	m_currentFrames++;
}

void GameEngine::init(const std::string& path)
{
	// go through the assets file and then create all assets
	std::string buf;
	std::ifstream fin(path);

	while (fin >> buf)
	{
		if (buf == "Texture")
		{
			std::string name, filePath;
			fin >> name >> filePath;
			m_assets.addTexture(name, filePath);
		}
		else if (buf == "Animation")
		{
			std::string animationName, textureName;
			int frameCount, animSpeed;
			fin >> animationName >> textureName >> frameCount >> animSpeed;
			//Animation anim(animationName, m_assets.getTexture(textureName), frameCount, animSpeed);
			//m_assets.addAnimation(animationName, anim);
			m_assets.addAnimation(animationName, textureName, frameCount, animSpeed);
		}
		else if (buf == "Font")
		{
			std::string name, filePath;
			fin >> name >> filePath;
			m_assets.addFont(name, filePath);
		}
		else
		{
			std::cout << "Something went wrong when loading assets file.\n";
			exit(-1);
		}
	}

	m_window.create(sf::VideoMode(1280, 768), "Definitely not Mario!");
	m_window.setFramerateLimit(144);

	changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
	return m_sceneMap[m_currentScene];
}

bool GameEngine::isRunning()
{
	return m_running && m_window.isOpen();
}

sf::RenderWindow& GameEngine::window()
{
	return m_window;
}

void GameEngine::run()
{
	while (isRunning())
	{
		update();
	}
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			quit();

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				std::cout << "screenshot saved to " << "test.png" << std::endl;
				sf::Texture texture;
				texture.create(m_window.getSize().x, m_window.getSize().y);
				texture.update(m_window);
				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "screenshot saved to " << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			// if the current scene does not have an action associated with this key, skip the event
			if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
				continue;

			// determine start or end action by whether it was key press or release
			std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			// look up the action and send the action to the scene
			currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
		}
	}
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	m_currentScene = sceneName;
	m_sceneMap.insert(std::pair<std::string, std::shared_ptr<Scene>>(sceneName, scene));
}

void GameEngine::quit()
{
	m_running = false;
}

const Assets& GameEngine::assets() const
{
	return m_assets;
}

const size_t GameEngine::getCurrentFrames() const
{
	return m_currentFrames;
}