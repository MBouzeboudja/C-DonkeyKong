#pragma once
#include "Entity.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();
	static std::shared_ptr<Entity> GetPlayer();
	static float GetClosestBlocFromPlayer(std::shared_ptr<Entity>);

public:
	static std::vector<std::shared_ptr<Entity>> m_Entities;
};

