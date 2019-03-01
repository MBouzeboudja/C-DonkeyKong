#include "pch.h"
#include "Level.h"
#include "EntityManager.h"


Level::Level()
{
}


Level::~Level()
{
}

int  Level::getMarioLevel()
{
	float positionY = EntityManager::GetPlayer()->m_sprite.getPosition().y;
	if (positionY < 100)
	{
		return 0;
	}
	if (positionY < 185 && positionY > 163)
	{
		return 1;
	}
	else if (positionY < 325 && positionY > 265)
	{
		return 2;
	}
	else if (positionY < 410 && positionY > 37)
	{
		return 3;
	}
	else if (positionY < 520 && positionY > 487)
	{
		return 4;
	}
	else if (positionY < 630 && positionY > 597)
	{
		return 5;
	}
	else return -1;
}
