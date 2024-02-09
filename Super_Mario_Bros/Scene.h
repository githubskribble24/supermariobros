#pragma once

#include "Action.h"
#include "GameEngine.h"
#include "EntityManager.h"

#include <memory>

typedef std::map<int, std::string> ActionMap;

class GameEngine;

// abstract class
class Scene
{

protected:

	GameEngine* m_game = nullptr;
	EntityManager m_entityManager;
	ActionMap     m_actionMap;
	bool		  m_paused = false;
	bool		  m_hasEnded = false;
	size_t		  m_currentFrame = 0;

	// pure virtual function, this declares that the class is abstract
	virtual void onEnd() = 0;
	void setPaused(bool paused);

public:
	//Scene();
	Scene(GameEngine* gameEngine);

	virtual void update() = 0;
	virtual void sRender() = 0;

	virtual void doAction(const Action& action) = 0;
	void simulate(const size_t frames);
	void registerAction(int inputKey, const std::string& actionName);

	size_t width() const;
	size_t height() const;
	size_t currentFrame() const;

	bool hasEnded() const;
	const ActionMap& getActionMap() const;
	ActionMap& getActionMap();
	void drawLine(const Vec2& p1, const Vec2& p2);
};