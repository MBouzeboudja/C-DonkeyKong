#pragma once

#define mIntroSoundPath "Media/Sounds/intro.wav"
#define mCoinSoundPath "Media/Sounds/coin.wav"
#define mDeathSoundPath "Media/Sounds/death.wav"
#define mWinSoundPath "Media/Sounds/win1.wav"

class SoundHelper
{
public:
	SoundHelper();
	~SoundHelper();
	void SoundPlay(std::string path);
private:
	sf::SoundBuffer mBuffer;
	sf::Sound mSound;
};



