#include "EntityManager.h"
#include "Entity.h"
#include <cassert>

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	// add entity to the list here
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);
	return entity;
}

void EntityManager::update()
{
	// check entities to add or remove here
	for (auto e : m_entitiesToAdd)
	{
		// add entity to entity list
		m_entityVec.push_back(e);

		// add entity to entity map
		m_entityMap[e->tag()].push_back(e);
	}

	removeDeadEntities();

	m_entitiesToAdd.clear();
}

EntityVec& EntityManager::getEntities()
{
	return m_entityVec;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	assert(m_entityMap.find(tag) != m_entityMap.end());
	return m_entityMap.at(tag);
}

bool EntityManager::entityExists(const std::string& tag)
{
	return (m_entityMap.find(tag) != m_entityMap.end());
}

void EntityManager::removeDeadEntities()
{
	EntityVec deadEntities;

	for (auto& e : m_entityVec)
	{
		if (!e->isActive())
			deadEntities.push_back(e);
		if (e->hasComponent<CLifeSpan>() == false)
			continue;
	}

	for (auto& e : deadEntities)
	{
		std::erase(m_entityVec, e);
		for (auto& [tag, mapEnt] : m_entityMap)
		{
			std::erase(mapEnt, e);
		}
	}
}