#pragma once

#define ECHELLE_COUNT 5
#define BLOCK_COUNT_X 8
#define BLOCK_COUNT_Y 6
#define BLOCK_SPACE 110.f
#define PIECE_COUNT_X 3
#define PIECE_COUNT_Y 6

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
	void ResetGame();
	void updateStatistics(sf::Time elapsedTime);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void HandleBarilMoves();
	void HandleBarilCreate();
	void HandleCollisionBarilPlayer();
	void DisplayGameOver();
	void HandleGameOver();
	void HandleCollisionPrincessePlayer();
	void DisplayWin();
	void HandleScoreLive();
	void HandleCollisionPiecePlayer(sf::Time elapsedTime);


private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow mWindow;
	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;

	sf::Text	mText;
	sf::Text	mScore;
	sf::Text	mLive;


	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool mIsJump;
	bool isGameOver;
	bool isWin;
	float pos = 0.f;
	int live;
	int score;
	bool canMoveVertical;


	sf::Texture	_TextureEchelle;
	sf::Sprite	_Echelle[ECHELLE_COUNT];

	sf::Texture	_TextureBlock;
	sf::Sprite	_Block[BLOCK_COUNT_X + 1][BLOCK_COUNT_Y];

	sf::Texture	_TextureWeapon;
	sf::Sprite	_Weapon;
	sf::Vector2u _sizeBlock;
	sf::Vector2u _sizeMario;

	sf::Texture _TextureKong;
	sf::Sprite	_Kong;
	sf::Font	_FontKong;
	sf::Vector2u _sizeKong;

	sf::Texture _TexturePrincesse;
	sf::Sprite	_Princesse;
	sf::Font	_FontPrincesse;
	sf::Vector2u _sizePrincesse;

	sf::Texture _TexturePiece;
	sf::Sprite	_Piece[PIECE_COUNT_X][PIECE_COUNT_Y];
	sf::Font	_FontPiece;
	sf::Vector2u _sizePiece;

	sf::Texture _TextureBaril;
	sf::Sprite	_Baril;
	//sf::Sprite	_Piece;
};

