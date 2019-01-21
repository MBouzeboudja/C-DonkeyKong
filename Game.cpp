#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"

const float Game::PlayerSpeed = 100.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(1000, 1000), "Donkey Kong 1981", sf::Style::Close)
	, mTexture()
	, mPlayer()
	, mFont()
	, mStatisticsText()
	, mStatisticsUpdateTime()
	, mStatisticsNumFrames(0)
	, mIsMovingUp(false)
	, mIsMovingDown(false)
	, mIsMovingRight(false)
	, mIsMovingLeft(false)
{
	mWindow.setFramerateLimit(160);
	mIsJump = false;
	live = 1;
	isGameOver = false;
	// Draw blocks

	_TextureBlock.loadFromFile("Media/Textures/Block.png");
	_sizeBlock = _TextureBlock.getSize();

	for (int i = 0; i < BLOCK_COUNT_X; i++)
	{
		for (int j = 0; j < BLOCK_COUNT_Y; j++)
		{
			_Block[i][j].setTexture(_TextureBlock);
			if (j % 2 != 0) {
				if (j == 1 && i < 3) _Block[i][j].setPosition(100.f + 70.f * (i + 1), 210.f);
				else
				{
					_Block[i][j].setPosition(100.f + 70.f * (i + 1), -10.f + BLOCK_SPACE * (j + 1) + i * 3.f);

				}
			}
			else {
				_Block[i][j].setPosition(170.f + 70.f * (i + 1), +10.f + BLOCK_SPACE * (j + 1) - i * 3.f);
			}

			std::shared_ptr<Entity> se = std::make_shared<Entity>();
			se->m_sprite = _Block[i][j];
			se->m_type = EntityType::block;
			se->m_size = _TextureBlock.getSize();
			se->m_position = _Block[i][j].getPosition();
			EntityManager::m_Entities.push_back(se);
		}
	}
	

	// Draw Echelles

	_TextureEchelle.loadFromFile("Media/Textures/Echelle.png");

	for (int i = 0; i < ECHELLE_COUNT; i++)
	{
		_Echelle[i].setTexture(_TextureEchelle);
		if(i%2!=0)
			_Echelle[i].setPosition(590.f , 4.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y );
		else
			_Echelle[i].setPosition(310.f ,5.f+BLOCK_SPACE * (i + 1) + _sizeBlock.y);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Echelle[i];
		se->m_type = EntityType::echelle;
		se->m_size = _TextureEchelle.getSize();
		se->m_position = _Echelle[i].getPosition();
		EntityManager::m_Entities.push_back(se);

	}

	// Draw Kong

	_TextureKong.loadFromFile("Media/Textures/Kong.png"); // Mario_small.png");
	_sizeKong = _TextureKong.getSize();
	_Kong.setTexture(_TextureKong);

	_Kong.setPosition(280.f, BLOCK_SPACE*2 - _sizeKong.y);

	std::shared_ptr<Entity> kong = std::make_shared<Entity>();
	kong->m_sprite = _Kong;
	kong->m_type = EntityType::Kong;
	kong->m_size = _TextureKong.getSize();
	kong->m_position = _Kong.getPosition();
	EntityManager::m_Entities.push_back(kong);

	// Draw Mario

	mTexture.loadFromFile("Media/Textures/Mario_small_transparent.png"); // Mario_small.png");
	_sizeMario = mTexture.getSize();
	mPlayer.setTexture(mTexture);
	sf::Vector2f posMario;
	posMario.x = 100.f + 70.f;
	posMario.y = -5.f + BLOCK_SPACE * 6  - _sizeMario.y;

	mPlayer.setPosition(posMario);

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	//Draw Baril
	_TextureBaril.loadFromFile("Media/Textures/Baril.png"); // Mario_small.png");
	_Baril.setTexture(_TextureBaril);

	_Baril.setPosition(380.f, 2*BLOCK_SPACE - _TextureBaril.getSize().y);

	std::shared_ptr<Entity> Baril = std::make_shared<Entity>();
	Baril->m_sprite = _Baril;
	Baril->m_type = EntityType::Baril;
	Baril->m_size = _TextureBaril.getSize();
	Baril->m_position = _Baril.getPosition();
	EntityManager::m_Entities.push_back(Baril);



	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);


}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;

			processEvents();
			update(TimePerFrame);
		}

		updateStatistics(elapsedTime);
		render();
	}
}



void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;

		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);

	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft) {
		movement.x -= PlayerSpeed;
		movement.y -= 3.8;
	}
	if (mIsMovingRight) {
		movement.x += PlayerSpeed;
		movement.y += 3.8;
	}

	

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type == EntityType::echelle)
		{
			sf::FloatRect boundBaril;
			boundBaril = entity->m_sprite.getGlobalBounds();

			sf::FloatRect boundPlayer;
			boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

			if (mIsMovingUp && boundPlayer.intersects(boundBaril)) {
				movement.y -= PlayerSpeed;
			}			
		}
		if (entity->m_type != EntityType::player)
		{
			continue;
		}

		sf::Vector2f mov = entity->m_sprite.getPosition();
	
		if (mov.x < 170.f) {
			entity->m_sprite.move(1.f, 0.f);
			mIsMovingLeft = false;
		}
		if (mov.x > 690.f) {
			entity->m_sprite.move(-1.f, 0.f);
			mIsMovingRight = false;
		}
		else entity->m_sprite.move(movement * elapsedTime.asSeconds());
		
		
	}
}

void Game::render()
{
	mWindow.clear();

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}
	mWindow.draw(mText);
	mWindow.draw(mStatisticsText);
	mWindow.display();
}

void Game::updateStatistics(sf::Time elapsedTime)
{
	mStatisticsUpdateTime += elapsedTime;
	mStatisticsNumFrames += 1;

	if (mStatisticsUpdateTime >= sf::seconds(1.0f))
	{
		mStatisticsText.setString(
			"Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
			"Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}

	//
	// Handle collision
	//

	if (mStatisticsUpdateTime >= sf::seconds(0.050f))
	{
		if (isGameOver) {
			return;
		}

		HandleBarilMoves();
		HandleBarilCreate();
		HandleCollisionBarilPlayer();
		DisplayGameOver();
		HandleGameOver();
	}
}

void Game::HandleGameOver()
{
	// Game Over ?
	
	if (EntityManager::GetPlayer()->m_enabled == false)
	{
		DisplayGameOver();
	}

	if (live == 0)
	{
		DisplayGameOver();
	}
}

void Game::DisplayGameOver()
{
	if (live==0) {
		mText.setFillColor(sf::Color::Green);
		mText.setFont(mFont);
		mText.setPosition(200.f, 200.f);
		mText.setCharacterSize(80);

		mText.setString("GAME OVER");
		isGameOver = true;
	}
}
void Game::HandleCollisionBarilPlayer()
{
	for (std::shared_ptr<Entity> Baril : EntityManager::m_Entities)
	{
		if (Baril->m_enabled == false)
		{
			continue;
		}

		if (Baril->m_type != EntityType::Baril)
		{
			continue;
		}

		sf::FloatRect boundBaril;
		boundBaril = Baril->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundBaril.intersects(boundPlayer) == true)
		{
			live --;
			Baril->m_enabled = false;
			DisplayGameOver();
			goto end;
		}
	}

end:
	//nop
	return;
}

void Game::HandleBarilCreate()
{
	pos = pos + 5.f;
	if (pos == 1300.f) {
		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureBaril);
		sw->m_sprite.setPosition(380.f, 2 * BLOCK_SPACE - _TextureBaril.getSize().y);
		sw->m_type = EntityType::Baril;
		sw->m_size = _TextureBaril.getSize();
		EntityManager::m_Entities.push_back(sw);
		pos = 0.f;
	}
	
}



void Game::HandleBarilMoves()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::Baril)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		if (y >  BLOCK_SPACE * (5 + 1) + 7 * 3.f) {
			entity->m_enabled = false;
		}
		
		if (entity->m_bLeftToRight == true) {
			x++;
			y = y + 0.04;
		}
		else {
			x--;
			y = y + 0.04;
		}

		if (x >= 710.f) //0)
		{
			entity->m_bLeftToRight = false;
			y = y + 90.f ;
		}
		if (x < 210.f) {
			entity->m_bLeftToRight = true;
			y = y + 90.f;
		}
		entity->m_sprite.setPosition(x, y);
	}
}
void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	sf::Clock clock;
	sf::Time elapsed1;

	if (key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;

	if (key == sf::Keyboard::Space)
	{

		if (isPressed == false)
		{

			mIsJump = false;
			EntityManager::GetPlayer()->m_sprite.setPosition(
				EntityManager::GetPlayer()->m_sprite.getPosition().x,
				EntityManager::GetPlayer()->m_sprite.getPosition().y + 50.f
			);
			return;
		}

		if (mIsJump== true) {
			sf::Time elapsed2 = clock.getElapsedTime();
			if (elapsed1.asSeconds()*2 < elapsed2.asSeconds()) {
				EntityManager::GetPlayer()->m_sprite.setPosition(
					EntityManager::GetPlayer()->m_sprite.getPosition().x,
					EntityManager::GetPlayer()->m_sprite.getPosition().y + 50.f
				);
			}
			return;
		}

		elapsed1 = clock.getElapsedTime();
		EntityManager::GetPlayer()->m_sprite.setPosition(
				EntityManager::GetPlayer()->m_sprite.getPosition().x,
				EntityManager::GetPlayer()->m_sprite.getPosition().y - 50.f
		);


		mIsJump = true;
		

	}
}
