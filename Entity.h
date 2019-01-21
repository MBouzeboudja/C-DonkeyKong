#pragma once

enum EntityType
{
	player,
	block,
	echelle,
	Kong,
	Baril,
	baril,
	princesse,
};

class Entity
{
public:
	Entity() { };
	~Entity() { };

public:
	sf::Sprite m_sprite;
	sf::Vector2u m_size;
	sf::Vector2f m_position;
	EntityType m_type;
	bool m_enabled = true;

	// Enemy only
	bool m_bLeftToRight = true;
	bool m_bTopToDown = true;
	int m_times = 0;
};

