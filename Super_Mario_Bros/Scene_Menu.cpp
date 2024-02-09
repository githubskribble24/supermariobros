#include "Components.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include "Vec2.h"
#include "Action.h"
#include "Scene.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"

#include <iostream>

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Menu::init()
{
	m_menuTitle.setFont(m_game->assets().getFont("Pixeled"));
	m_menuText.setFont(m_game->assets().getFont("Pixeled"));

	m_menuTitle.setString("Definitely not Mario");

	m_menuStrings.push_back("Level 1");
	m_levelPaths.push_back("levels/level1.txt");
	m_menuStrings.push_back("Level 2");
	m_menuStrings.push_back("Level 3");

	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::Enter, "SELECT");
	
}

void Scene_Menu::update()
{
	// Render the text etc
	sRender();
	// update entity manager
	m_entityManager.update();
}


void Scene_Menu::sRender()
{
	m_game->window().clear();
	
	m_menuTitle.setPosition(15.0f, 15.0f);
	m_game->window().draw(m_menuTitle);

	float startX = 15.0f,
		  startY = 15.0f;
	for (size_t i = 0; i<m_menuStrings.size(); i++)
	{
		std::string& text = m_menuStrings[i];
		startY += 45.0f;

		// change color if this current option is the m_menuIndex
		m_menuText.setFillColor((m_menuIndex == i) ? sf::Color::Green : sf::Color::White);
		m_menuText.setString(text);
		m_menuText.setPosition(startX, startY);
		m_game->window().draw(m_menuText);

	}

	m_game->window().display();
}

void Scene_Menu::doAction(const Action& action)
{
	// user input W, S
	if (action.getType() == "START")
	{
		if (action.getName() == "UP")
		{
			if (m_menuIndex != 0)
				m_menuIndex -= 1;
		}
		else if (action.getName() == "DOWN")
		{
			if (m_menuIndex != m_menuStrings.size() - 1)
				m_menuIndex += 1;
		}
		else if (action.getName() == "SELECT")
		{
			std::cout << "m_levelPaths: " << m_levelPaths[m_menuIndex] << "; \n";
			m_game->changeScene(m_menuStrings[m_menuIndex], std::make_shared<Scene_Play>(m_game, m_levelPaths.at(m_menuIndex)), true);
		}
	}

	if (action.getType() == "END")
	{

	}
}

void Scene_Menu::onEnd()
{
	std::cout << "Gotten out of menu\n";
}

void Scene_Menu::registerAction(sf::Keyboard::Key inputKey, const std::string& actionName)
{
	m_actionMap[inputKey] = actionName;
}