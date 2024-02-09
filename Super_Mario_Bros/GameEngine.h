#pragma once

#include "Scene.h"
#include "Assets.h"
#include <SFML/Graphics.hpp>

#include <memory>

class Scene;

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{

protected:
	sf::RenderWindow m_window;
	Assets			 m_assets;
	std::string		 m_currentScene;
	SceneMap		 m_sceneMap;
	size_t			 m_simulationSpeed = 1;
	bool			 m_running = true;
	size_t				 m_currentFrames = 0;

	void init(const std::string& path);
	void update();

	void sUserInput();

	std::shared_ptr<Scene> currentScene();

public:
	GameEngine(const std::string& path);

	void changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);
	const size_t getCurrentFrames() const;

	void quit();
	void run();

	sf::RenderWindow& window();
	const Assets& assets() const;
	bool isRunning();
};