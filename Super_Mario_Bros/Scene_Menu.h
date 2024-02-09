#pragma once

#include "Components.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include "Vec2.h"
#include "Action.h"
#include "Scene.h"

#include <iostream>
#include <vector>

//virtual void update() = 0;
//virtual void sDoAction(const Action& action) = 0;
//virtual void sRender() = 0;
//virtual void doAction(const Action& action) = 0;
//virtual void onEnd() = 0;

class Scene_Menu : public Scene
{
protected:
	std::vector<std::string> m_menuStrings;
	sf::Text m_menuText;
	sf::Text m_menuTitle;
	std::vector<std::string> m_levelPaths;
	size_t m_menuIndex{ 0 };

public:
	Scene_Menu(GameEngine* gameEngine);

	void init();
	void update() override;
	void registerAction(sf::Keyboard::Key inputKey, const std::string& actionName);

	// Systems
	void sRender() override;
	void doAction(const Action& action) override;

	void onEnd() override;
};