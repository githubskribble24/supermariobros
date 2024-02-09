#pragma once

#include <map>
#include <memory>
#include <SFML/Graphics.hpp>

#include "Scene.h"
#include "GameEngine.h"
#include "EntityManager.h"
#include "Vec2.h"
#include "Action.h"

class Scene_Play : public Scene
{
	struct PlayerConfig
	{
		float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
		std::string weapon;
	};

protected:
	sf::Text m_nameText;
	sf::Text m_scoreText;
	sf::Text m_coinsText;
	std::shared_ptr<Entity> m_player;
	std::string m_levelPath;
	PlayerConfig m_playerConfig;
	int m_score = 0;
	int m_coins = 0;
	sf::Text m_gridText;
	Vec2 m_gridSize{ 64, 64 };
	bool m_drawTextures = true;
	bool m_drawCollision = false;
	bool m_drawGrid = false;
	bool m_grounded = false;

public:
	EntityManager m_entityManager;

	Scene_Play(GameEngine* gameEngine, const std::string& levelPath);
	Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);
	void loadLevel(const std::string& fileName);

	void update();
	void spawnBullet(std::shared_ptr<Entity> entity);
	void init(const std::string& levelPath);
	void registerAction(sf::Keyboard::Key key, const std::string& state);

	void spawnPlayer();

	// Systems
	void sMovement();
	void sLifespan();
	void sCollision();
	void sAnimation();
	void sRender();
	void doAction(const Action& action);
	void onEnd();
};