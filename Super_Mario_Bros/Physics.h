#pragma once

#include "Entity.h"

class Physics
{
public:
	static Vec2 GetOverlap(const std::shared_ptr<Entity> a, const std::shared_ptr<Entity> b);
	static Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};