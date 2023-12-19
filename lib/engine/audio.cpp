#include "engine/audio.h"
#include "engine/files.h"
#include "engine/geometry.h"

#include "stb/stb_vorbis.h"

std::vector<Sound> sounds;

std::vector<SoundData> soundDatas;
std::vector<SoundConfig> soundConfigs;

void Audio_loadSoundData(const char *path, const char *name){

	SoundData soundData;

	String_set(soundData.name, name, STRING_SIZE);

	int sampleRate;
	soundData.n_samples = stb_vorbis_decode_filename(path, &soundData.n_channels, &sampleRate, &soundData.data);

	if(soundData.n_samples < 0){
		printf("Could not load audio file %s\n", path);
	}

	soundDatas.push_back(soundData);

}

void Audio_loadSoundConfig(const char *path){

	soundConfigs.clear();

	int n_lines;
	String *fileLines = getFileLines_mustFree(path, &n_lines);

	for(int i = 0; i < n_lines; i++){

		if(fileLines[i] == ":Sound"){

			char *ptr;

			SoundConfig config;

			String_set(config.name, fileLines[i + 1], SMALL_STRING_SIZE);

			config.volume = strtof(fileLines[i + 2], &ptr);

			config.n_variations = strtol(fileLines[i + 3], &ptr, 10);

			soundConfigs.push_back(config);

		}

	}

	free(fileLines);

}

void Audio_playSoundData(SoundData *soundData_p, float volume){

	Sound sound;
	sound.soundData_p = soundData_p;
	sound.n_playedSamples = 0;
	sound.volume = volume;

	sounds.push_back(sound);

}

void Audio_playSound(const char *name, float volume){

	for(int i = 0; i < soundDatas.size(); i++){
		if(strcmp(soundDatas[i].name, name) == 0){
			Audio_playSoundData(&soundDatas[i], volume);
			return;
		}
	}

	printf("Could not find sound with name: %s\n", name);

}

void Audio_playSoundConfig(const char *name, float volume){

	SoundConfig config;

	for(int i = 0; i < soundConfigs.size(); i++){
		if(soundConfigs[i].name == name){
			config = soundConfigs[i];
			break;
		}
	}

	int variation = (int)(1.0 + getRandom() * (float)config.n_variations);

	String variationName;
	String_set(variationName, config.name, STRING_SIZE);
	String_append(variationName, "-");
	String_append_int(variationName, variation);

	Audio_playSound(variationName, volume * config.volume);

}
