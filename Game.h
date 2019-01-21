#pragma once

#define ECHELLE_COUNT 5
#define BLOCK_COUNT_X 8
#define BLOCK_COUNT_Y 6
#define BLOCK_SPACE 110.f
#define FIRST_SPACE 300.f

class Game
{
public:
	Game();

	~Game() { };
	void run();

private:
	void processEvents();
	void update(sf::Time elapsedTime);
	void render();

	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void HandleBarilMoves();
	void HandleBarilCreate();
	void HandleCollisionBarilPlayer();
	void DisplayGameOver();
	void HandleGameOver();

private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;

	sf::Text	mText;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool mIsJump;
	bool isGameOver;
	float pos = 0.f;
	int live;


	sf::Texture	_TextureEchelle;
	sf::Sprite	_Echelle[ECHELLE_COUNT];

	sf::Texture	_TextureBlock;
	sf::Sprite	_Block[BLOCK_COUNT_X][BLOCK_COUNT_Y];

	sf::Texture	_TextureWeapon;
	sf::Sprite	_Weapon;
	sf::Vector2u _sizeBlock;
	sf::Vector2u _sizeMario;

	sf::Texture _TextureKong;
	sf::Sprite	_Kong;
	sf::Font	_FontKong;
	sf::Vector2u _sizeKong;

	sf::Texture _TextureBaril;
	sf::Sprite	_Baril;
};

