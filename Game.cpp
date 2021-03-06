#include "pch.h"
#include "StringHelpers.h"
#include "Game.h"
#include "EntityManager.h"
#include "Level.h"
#include "SoundHelper.h"

const float Game::PlayerSpeed = 80.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
	: mWindow(sf::VideoMode(1000, 800), "Donkey Kong 1981", sf::Style::Close)
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
	, canMoveVertical(false)
	, moveHorizontal(1)
	, moveVertical(1)
	, kong_position(1)
	, _sound_helper ()
{
	mWindow.setFramerateLimit(160);
	mIsJump = false;
	live = 3;
	isGameOver = false;
	isWin = false;
	score = 0;

	// Draw blocks
	_TextureBlock.loadFromFile("Media/Textures/Block.png");
	_TextureBlock.setSmooth(true);
	_sizeBlock = _TextureBlock.getSize();

	for (int i = 0; i < BLOCK_COUNT_X; i++)
	{
		for (int j = 0; j < BLOCK_COUNT_Y; j++)
		{
			_Block[i][j].setTexture(_TextureBlock);
			if (j == 0) {
				if (i >= 3 && i < 6) _Block[i][j].setPosition(100.f + 70.f * (i + 1), BLOCK_SPACE+20.f);
				else
					continue;
			}
			else if (j % 2 != 0) {
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

			//Draw an other bloc in the last raw
			if(i == (BLOCK_COUNT_X-1) && j == (BLOCK_COUNT_Y-1))
			{
				_Block[i+1][j].setTexture(_TextureBlock);
				_Block[i+1][j].setPosition(170.f + 70.f * (i + 1), -10.f + BLOCK_SPACE * (j + 1) + i * 3.f);
				std::shared_ptr<Entity> se = std::make_shared<Entity>();
				se->m_sprite = _Block[i+1][j];
				se->m_type = EntityType::block;
				se->m_size = _TextureBlock.getSize();
				se->m_position = _Block[i+1][j].getPosition();
				EntityManager::m_Entities.push_back(se);
			}
		}
	}
	

	// Draw Echelles

	_TextureEchelle.loadFromFile("Media/Textures/Echelle.png");
	_TextureEchelle.setSmooth(true);
	for (int i = 0; i < ECHELLE_COUNT; i++)
	{	
		_Echelle[i].setTexture(_TextureEchelle);
		if(i==0) _Echelle[i].setPosition(540.f, -14.f+BLOCK_SPACE + _sizeBlock.y);
		else if(i%2!=0)
			_Echelle[i].setPosition(590.f , -25.f + BLOCK_SPACE * (i + 1) + _sizeBlock.y );
		else
			_Echelle[i].setPosition(310.f ,-25.f+BLOCK_SPACE * (i + 1) + _sizeBlock.y);

		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = _Echelle[i];
		se->m_type = EntityType::echelle;
		se->m_size = _TextureEchelle.getSize();
		se->m_position = _Echelle[i].getPosition();
		EntityManager::m_Entities.push_back(se);

	}

	// Draw Kong

	_TextureKong.loadFromFile("Media/Textures/test.png");
	_sizeKong = _TextureKong.getSize();
	_TextureKong.setSmooth(true);
	_Kong.setTexture(_TextureKong);
	_Kong.setTextureRect(sf::IntRect(252, 7, 48, 35));
	_Kong.setScale(2, 2);

	_Kong.setPosition(260.f, BLOCK_SPACE * 2 - 77);

	std::shared_ptr<Entity> kong = std::make_shared<Entity>();
	kong->m_sprite = _Kong;
	kong->m_type = EntityType::Kong;
	kong->m_size = _TextureKong.getSize();
	kong->m_position = _Kong.getPosition();
	EntityManager::m_Entities.push_back(kong);

	// Draw Princesse

	_TexturePrincesse.loadFromFile("Media/Textures/princesse.png"); // Mario_small.png");
	_TexturePrincesse.setSmooth(true);
	_sizePrincesse = _TexturePrincesse.getSize();
	_Princesse.setTexture(_TexturePrincesse);

	_Princesse.setPosition(100.f + 70.f * 4, BLOCK_SPACE +30.f - _sizePrincesse.y);

	std::shared_ptr<Entity> princesse = std::make_shared<Entity>();
	princesse->m_sprite = _Princesse;
	princesse->m_type = EntityType::princesse;
	princesse->m_size = _TexturePrincesse.getSize();
	princesse->m_position = _Princesse.getPosition();
	EntityManager::m_Entities.push_back(princesse);

	// Draw Mario

	mTexture.loadFromFile("Media/Textures/MarioTextures.png"); // Mario_small.png");
	_sizeMario = sf::Vector2u(12,16);
	mPlayer.setTexture(mTexture);
	mPlayer.setTextureRect(sf::IntRect(160, 0, 12, 16));
	mPlayer.setScale(3, 3);
	mTexture.setSmooth(false);
	sf::Vector2f posMario;
	posMario.x = 170.f;
	posMario.y = BLOCK_SPACE * 6 - 45.f -_sizeMario.y;

	mPlayer.setPosition(posMario);

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	//Draw Baril
	_TextureBaril.loadFromFile("Media/Textures/Baril.png");
	_Baril.setTexture(_TextureBaril);
	

	//DRaw baril static 
	_TextureBarilStatic.loadFromFile("Media/Textures/barilSatatic.png");
	_BarilStatic.setTexture(_TextureBarilStatic);
	_BarilStatic.setTextureRect(sf::IntRect(112, 263, 12, 20));
	_BarilStatic.setScale(4,4);
	_TextureBarilStatic.setSmooth(false);
	sf::Vector2f posBarilStatic;
	_BarilStatic.setPosition(205.f, BLOCK_SPACE * 2 - 75);
	std::shared_ptr<Entity> barilStatic = std::make_shared<Entity>();
	barilStatic->m_sprite = _BarilStatic;
	barilStatic->m_type = EntityType::barilStatic;
	barilStatic->m_size = _TextureBarilStatic.getSize();
	barilStatic->m_position = _BarilStatic.getPosition();
	EntityManager::m_Entities.push_back(barilStatic);

	//Draw piece
	_TexturePiece.loadFromFile("Media/Textures/piece.png"); 
	_sizePiece = _TextureBlock.getSize();

	for (int i = 0; i < PIECE_COUNT_X; i++)
	{
		for (int j = 1; j < PIECE_COUNT_Y; j++)
		{
			_Piece[i][j].setTexture(_TexturePiece);
			if (j == 1) {
				if (i > 0 && i <= 3) {
					_Piece[i][j].setPosition(100.f + 70.f * (i*3+2), -60.f + BLOCK_SPACE * (j + 1) + (i * 3 + 2) * 3.f);
				}
				else
					continue;
			}
			else if (j % 2 != 0) {
				if (i != 1) {
					_Piece[i][j].setPosition(100.f+ 70.f * (i * 3 + 2), -60.f + BLOCK_SPACE * (j + 1) + (i * 3 + 2) * 3.f);
				}
				else
				{
					continue;
				}
			}
			else {
				_Piece[i][j].setPosition(190.f + 70.f * (i*3 + 1), -30.f + BLOCK_SPACE * (j + 1) - (i * 3 + 2) * 3.f);
			}

			std::shared_ptr<Entity> piece = std::make_shared<Entity>();
			piece->m_sprite = _Piece[i][j];
			piece->m_type = EntityType::piece;
			piece->m_size = _TexturePiece.getSize();
			piece->m_position = _Piece[i][j].getPosition();
			EntityManager::m_Entities.push_back(piece);
		}
	}

	// Draw Statistic Font 

	mFont.loadFromFile("Media/Sansation.ttf");
	mStatisticsText.setString("Welcome to Donkey Kong 1981");
	mStatisticsText.setFont(mFont);
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10);
	
	//Draw score
	mScore.setFillColor(sf::Color::Green);
	mScore.setFont(mFont);
	mScore.setPosition(20.f, 100.f);
	mScore.setCharacterSize(20);
	mScore.setString(std::to_string(score));
	//Draw live
	mLive.setFillColor(sf::Color::Green);
	mLive.setFont(mFont);
	mLive.setPosition(20.f, 130.f);
	mLive.setCharacterSize(20);
	mLive.setString(std::to_string(live));
	_sound_helper.SoundPlay(mIntroSoundPath);

}

void Game::ResetGame()
{

	mPlayer.setTexture(mTexture);
	mPlayer.setTextureRect(sf::IntRect(160, 0, 12, 16));
	mPlayer.setScale(3, 3);
	mTexture.setSmooth(false);
	sf::Vector2f posMario;
	posMario.x = 170.f;
	posMario.y = BLOCK_SPACE * 6 - 45.f - _sizeMario.y;

	mPlayer.setPosition(posMario);

	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);
}

void Game::run()
{
	sf::Clock clock;
	sf::Clock clock1;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			processEvents();
			if (isWin == false) {
				update(TimePerFrame);
			}
			if(clock1.getElapsedTime().asSeconds() > 0.5f)
			{
				KongAnimation();
				clock1.restart();
			}
		}
		updateStatistics(elapsedTime);
		render();
	}
}

void Game::processEvents()
{
	sf::Event event;
	while (mWindow.pollEvent(event) && isGameOver == false)
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
	std::shared_ptr<Entity> entity = EntityManager::GetPlayer();
	if(entity == nullptr)
	{
		return;
	}
	sf::Vector2f movement(0.f, 0.f);
	std::shared_ptr<Entity> player = EntityManager::GetPlayer();
	if(player == nullptr || !player->m_enabled)
	{
		return;
	}
	canMoveVertical = false;
	if (mIsMovingDown || mIsMovingUp) {
		for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
		{
			if (entity->m_enabled == false)
			{
				continue;
			}
			if (entity->m_type == EntityType::echelle)
			{
				sf::Vector2f marioOrigin = EntityManager::GetPlayer()->m_sprite.getOrigin();
				sf::FloatRect boundEchelle;
				boundEchelle = entity->m_sprite.getGlobalBounds();
				boundEchelle.width -= 18;
				if (mIsMovingDown) {
					boundEchelle.top -= 48;
				}
				boundEchelle.left += 8;
				sf::FloatRect boundPlayer;
				boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();
				boundPlayer.width -= 46;
				boundPlayer.left += 23;

				if (boundEchelle.intersects(boundPlayer)) {
					canMoveVertical = true;
				}
			}
			if (entity->m_type != EntityType::player)
			{
				continue;
			}
		}
	}
	int LevelMario = Level::getMarioLevel();
	sf::Vector2f mov = player->m_sprite.getPosition();
	if (mIsMovingUp && canMoveVertical)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown && canMoveVertical)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft && Level::getMarioLevel() != -1) {
		movement.x -= PlayerSpeed;
		if (LevelMario > 0 ) {
			if(LevelMario % 2 == 0)
			{
				movement.y += MOVE_OFFSET_y;
			}	
			if(LevelMario % 2 != 0)
			{
				movement.y -= MOVE_OFFSET_y;;
			}
		}
	}
	if (mIsMovingRight && Level::getMarioLevel() != -1) {
		movement.x += PlayerSpeed;
		if (LevelMario > 0) {
			if (LevelMario % 2 == 0)
			{
				movement.y -= MOVE_OFFSET_y;
			}
			if (LevelMario % 2 != 0)
			{
				movement.y += MOVE_OFFSET_y;;
			}
		}
	}
	if(mov.x > 550 && LevelMario == 0)
	{
		entity->m_sprite.move(-1.f, 0.f);
		mIsMovingRight = false;
		return;
	}

	if (mov.x < 170.f && LevelMario % 2 !=0) {
		entity->m_sprite.move(1.f, 0.f);
		mIsMovingLeft = false;
		return;
	}
	if (mov.x < 240.f && LevelMario % 2 == 0) {
		entity->m_sprite.move(1.f, 0.f);
		mIsMovingLeft = false;
		return;
	}
	if (mov.x > 680.f && (LevelMario == 1 || LevelMario == 3)) {
		entity->m_sprite.move(-1.f, 0.f);
		mIsMovingRight = false;
		return;
	}
	if (mov.x > 760.f) {
		entity->m_sprite.move(-1.f, 0.f);
		mIsMovingRight = false;
		return;
	}
	else entity->m_sprite.move(movement * elapsedTime.asSeconds());
		
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
	mWindow.draw(mScore);
	mWindow.draw(mLive);
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
		if (isGameOver || isWin ) {
			return;
		}

		
		HandleScoreLive();
		HandleGameOver();
		HandleBarilMoves();
		HandleBarilCreate();
		HandleCollisionBarilPlayer();
		HandleCollisionPrincessePlayer();
		HandleCollisionPiecePlayer(elapsedTime);
		
	}
}


void Game::HandleScoreLive()
{
		mScore.setString("Score : " + std::to_string(score));
		mLive.setString("Lives : " + std::to_string(live));
}

void Game::HandleCollisionPiecePlayer(sf::Time elapsedTime)
{
	for (std::shared_ptr<Entity> piece : EntityManager::m_Entities)
	{
		if (piece->m_enabled == false)
		{
			continue;
		}

		if (piece->m_type != EntityType::piece)
		{
			continue;
		}

		sf::FloatRect boundPiece;
		boundPiece = piece->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		std::shared_ptr<Entity> player = EntityManager::GetPlayer();
		if (player != NULL) {
			boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();
			if (boundPiece.intersects(boundPlayer) == true)
			{
				_sound_helper.SoundPlay(mCoinSoundPath);
				piece->m_enabled = false;
				// make mario bigger when he make collision with *piece.
			   // TODO: do this below
				player->m_sprite.scale(1.04f, 1.f);
				score += 100;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleGameOver()
{
	// Game Over ?
	
	if (EntityManager::GetPlayer() == nullptr)
	{
		DisplayGameOver();
	}

	if (live == 0)
	{
		DisplayGameOver();
	}
}

void Game::DisplayWin()
{

		mText.setFillColor(sf::Color::Green);
		mText.setFont(mFont);
		mText.setPosition(200.f, 400.f);
		mText.setCharacterSize(100);

		mText.setString("Champion!!!");
		isWin = true;
		_sound_helper.SoundPlay(mWinSoundPath);
	
}
void Game::DisplayGameOver()
{
	if (live == 0) {
		mText.setFillColor(sf::Color::Red);
		mText.setFont(mFont);
		mText.setPosition(200.f, 400.f);
		mText.setCharacterSize(100);
		mText.setString("GAME OVER");
		_sound_helper.SoundPlay(mDeathSoundPath);
		isGameOver = true;

	}
	else
	{
		ResetGame();
	}
}
void Game::HandleCollisionPrincessePlayer()
{
	for (std::shared_ptr<Entity> princesse : EntityManager::m_Entities)
	{
		if (princesse->m_enabled == false)
		{
			continue;
		}

		if (princesse->m_type != EntityType::princesse)
		{
			continue;
		}

		sf::FloatRect boundPrincesse;
		boundPrincesse = princesse->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		if (EntityManager::GetPlayer() != nullptr) {
			boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

			if (boundPrincesse.intersects(boundPlayer) == true)
			{
				DisplayWin();
				goto end;
			}
		}
	}

end:
	//nop
	return;
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
		if (EntityManager::GetPlayer() != nullptr) {
			boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();
			boundPlayer.height -=20;
			boundPlayer.width -=20;
			if (boundBaril.intersects(boundPlayer) == true)
			{
				live--;
				score -= 50;
				Baril->m_enabled = false;
				EntityManager::GetPlayer()->m_enabled = false;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleBarilCreate()
{
	pos = pos + 5.f;
	if (pos == 1600.f) {
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
	std::shared_ptr<Entity> player = EntityManager::GetPlayer();
	if (player != nullptr) {
		if (key == sf::Keyboard::Up) {
			mIsMovingUp = isPressed;
			if (moveVertical && isPressed) {
				player->m_sprite.setTextureRect(sf::IntRect(160, 40, 16, 16));
				moveVertical = 0;
			}
			else
			{
				player->m_sprite.setTextureRect(sf::IntRect(121, 40, 16, 16));
				moveVertical = 1;
			}
			return;
		}
		else if (key == sf::Keyboard::Down) {
			mIsMovingDown = isPressed;
			if (moveVertical  && isPressed) {
				player->m_sprite.setTextureRect(sf::IntRect(160, 40, 16, 16));
				moveVertical = 0;
			}
			else
			{
				player->m_sprite.setTextureRect(sf::IntRect(121, 40, 16, 16));
				moveVertical = 1;
			}
			return;
		}
		else if (key == sf::Keyboard::Left) {
			mIsMovingLeft = isPressed;
			if (moveHorizontal && isPressed) {
				player->m_sprite.setTextureRect(sf::IntRect(80, 0, 16, 16));
				moveHorizontal = 0;
			}
			else
			{
				player->m_sprite.setTextureRect(sf::IntRect(120, 0, 16, 16));
				moveHorizontal = 1;
			}
			return;
		}
		else if (key == sf::Keyboard::Right) {
			mIsMovingRight = isPressed;
			if (moveHorizontal && isPressed) {
				player->m_sprite.setTextureRect(sf::IntRect(200, 0, 16, 16));
				moveHorizontal = 0;
			}
			else
			{

				player->m_sprite.setTextureRect(sf::IntRect(160, 0, 16, 16));
				moveHorizontal = 1;
			}
			return;
		}

		else if (key == sf::Keyboard::Space) {

			if (isPressed == false)
			{
				mIsJump = isPressed;
				EntityManager::GetPlayer()->m_sprite.setPosition(
					player->m_sprite.getPosition().x,
					player->m_sprite.getPosition().y + 50.f
				);
				return;
			}

			if (mIsJump == true) {
				return;
			}
			std::shared_ptr<Entity> player = EntityManager::GetPlayer();
			player->m_sprite.setPosition(
				player->m_sprite.getPosition().x,
				player->m_sprite.getPosition().y - 50.f
			);
			mIsJump = true;
		}
	}
}

void Game::KongAnimation()
{
	std::shared_ptr<Entity> kong = EntityManager::GetKong();
	if(kong_position == 1)
	{
		kong->m_sprite.setTextureRect(sf::IntRect(252, 7, 48, 35));
		kong_position = 2;
	}
	else if(kong_position == 2)
	{
		kong->m_sprite.setTextureRect(sf::IntRect(106, 7, 48, 35));
		kong_position = 3;
	}	
	else if(kong_position == 3)
	{
		kong->m_sprite.setTextureRect(sf::IntRect(7, 7, 48, 35));
		kong_position = 4;
	}
	else if (kong_position = 4)
	{
		kong->m_sprite.setTextureRect(sf::IntRect(156, 7, 48, 35));
		kong_position = 1;
	}
}



