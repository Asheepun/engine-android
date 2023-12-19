#ifndef AUDIO_H_
#define AUDIO_H_

#include "engine/strings.h"

#include <vector>

struct SoundConfig{
	String name;
	float volume;
	int n_variations;
};

struct SoundData{
	int16_t *data;
	int n_channels;
	int n_samples;
	String name;
};

struct Sound{
	SoundData *soundData_p;
	float volume;
	int n_playedSamples;
};

extern std::vector<Sound> sounds;
extern std::vector<SoundConfig> soundConfigs;

void Audio_init();

void Audio_loadSoundData(const char *, const char *);

void Audio_loadSoundConfig(const char *);

void Audio_playSoundData(SoundData *, float);

void Audio_playSound(const char *, float);

void Audio_playSoundConfig(const char *, float);

void Audio_setFrequency(float);

#endif
