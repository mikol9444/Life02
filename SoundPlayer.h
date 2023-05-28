#pragma once
class SoundPlayer
{
public:
	SoundPlayer(); //Default constructor
	~SoundPlayer(); //Default destructor

	static void Play(const char* path, bool loop, int volume); //load wav audiofile and pass a volume between 0-100, loop not working -.-
};

