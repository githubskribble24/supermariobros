#include "Physics.h"
#include "Components.h"
#include <iostream>
#include <cmath>

Vec2 Physics::GetOverlap(const std::shared_ptr<Entity> a, const std::shared_ptr<Entity> b)
{
	if (!a->hasComponent<CBoundingBox>() && !b->hasComponent<CBoundingBox>())
		return Vec2(0, 0);

	if (!a->hasComponent<CTransform>() && !b->hasComponent<CTransform>())
		return Vec2(0, 0);

	auto& aPos = a->getComponent<CTransform>().pos;
	auto& bPos = b->getComponent<CTransform>().pos;
	auto& aSize = a->getComponent<CBoundingBox>().size;
	auto& bSize = b->getComponent<CBoundingBox>().size;

	Vec2 delta = { abs(aPos.x - bPos.x), abs(aPos.y - bPos.y) };
	int ox = (aSize.x / 2 + bSize.x / 2) - delta.x;
	int oy = (aSize.y / 2 + bSize.y / 2) - delta.y;
	Vec2 overlap(ox, oy);

	return overlap;
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	if (!a->hasComponent<CBoundingBox>() && !b->hasComponent<CBoundingBox>())
		return Vec2(0, 0);

	if (!a->hasComponent<CTransform>() && !b->hasComponent<CTransform>())
		return Vec2(0, 0);

	auto& aPos = a->getComponent<CTransform>().prevPos;
	auto& bPos = b->getComponent<CTransform>().prevPos;
	auto& aSize = a->getComponent<CBoundingBox>().size;
	auto& bSize = b->getComponent<CBoundingBox>().size;

	Vec2 delta = { abs(aPos.x - bPos.x), abs(aPos.y - bPos.y) };
	int ox = (aSize.x / 2) + (bSize.x / 2) - delta.x;
	int oy = (aSize.y / 2) + (bSize.y / 2) - delta.y;
	Vec2 overlap(ox, oy);

	return overlap;
}