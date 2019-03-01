#include "pch.h"
#include "EntityManager.h"

std::vector<std::shared_ptr<Entity>> EntityManager::m_Entities;

EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

std::shared_ptr<Entity> EntityManager::GetPlayer()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::player)
		{
			return entity;
		}
	}

	return nullptr;
}

float EntityManager::GetClosestBlocFromPlayer(std::shared_ptr<Entity> player)
{
	const float playerPositionY = player->m_sprite.getPosition().y;
	float closestBlocFromPlayerY = 900000;

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_type != EntityType::block)
		{
			continue;
		}
		if (std::abs(entity->m_sprite.getPosition().y - playerPositionY) < abs(playerPositionY - closestBlocFromPlayerY)) {
			closestBlocFromPlayerY = entity->m_sprite.getPosition().y;
		}
	}
	std::cout << closestBlocFromPlayerY;
	return closestBlocFromPlayerY;
}

std::shared_ptr<Entity> EntityManager::GetKong()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::Kong)
		{
			return entity;
		}
	}

	return nullptr;
}
std::shared_ptr<Entity> EntityManager::GetBarile()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::Baril)
		{
			return entity;
		}
	}

	return nullptr;
}



