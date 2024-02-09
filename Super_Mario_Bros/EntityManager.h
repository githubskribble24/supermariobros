#pragma once

#include "Components.h"
#include "Entity.h"

#include <vector>
#include <map>

class Entity;

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
	EntityVec m_entityVec;
	EntityMap m_entityMap;
	EntityVec m_entitiesToAdd;
	size_t m_totalEntities = 0;

public:
	EntityManager() {}

	std::shared_ptr<Entity> addEntity(const std::string& tag);
	void update();
	void removeDeadEntities();
	EntityVec& getEntities();
	EntityVec& getEntities(const std::string& tag);
	bool entityExists(const std::string& tag);
};