#include "pch.h"
#include "SoundHelper.h"


SoundHelper::SoundHelper()
{
}

SoundHelper::~SoundHelper()
{
}
void SoundHelper::SoundPlay(std::string path)
{
	if (mBuffer.loadFromFile(path))
	{
		mSound.setBuffer(mBuffer);
		mSound.play();
		mSound.setPlayingOffset(sf::seconds(2.f));
	}
}
