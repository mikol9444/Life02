#include "pch.h"
#include "SoundPlayer.h"
#include <mmsystem.h>

SoundPlayer::SoundPlayer()
{
    // Constructor implementation (if needed)
}

SoundPlayer::~SoundPlayer()
{
    // Destructor implementation (if needed)
}

void SoundPlayer::Play(const char* path, bool loop, int volume)
{
    DWORD soundFlags = SND_NODEFAULT | SND_ASYNC | SND_FILENAME;

    if (loop)
    {
        soundFlags |= SND_LOOP;
    }

    ::PlaySoundA(path, NULL, soundFlags);

    // Adjust the volume (0-100)
    int adjustedVolume = (volume * 0xFFFF) / 100;
    ::waveOutSetVolume(NULL, MAKELONG(adjustedVolume, adjustedVolume));
}