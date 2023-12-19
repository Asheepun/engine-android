#ifdef __linux__

#include "engine/audio.h"

#include <pulse/simple.h>
#include <pulse/error.h>

#include "stdio.h"
#include "math.h"
#include "pthread.h"
#include <vector>

pa_simple *s;
pa_sample_spec ss;

void *audioDataLoop(void *);

const int SAMPLE_RATE = 44100;

float frequency = 1 / 10.0;

void Audio_init(){

	ss.format = PA_SAMPLE_S16NE;
	ss.channels = 1;
	ss.rate = SAMPLE_RATE;

	s = pa_simple_new(
		NULL,               // Use the default server.
		"Game",           // Our application's name.
		PA_STREAM_PLAYBACK,
		NULL,               // Use the default device.
		"Audio",            // Description of our stream.
		&ss,                // Our sample format.
		NULL,               // Use default channel map
		NULL,               // Use default buffering attributes.
		NULL               // Ignore error code.
	);

	pthread_t audioDataLoopThread;
	pthread_create(&audioDataLoopThread, NULL, audioDataLoop, NULL);

}

void *audioDataLoop(void *args){

	float MASTER_VOLUME = 0.1;
	//MASTER_VOLUME = 0.0;

	int BUFFER_LENGTH = 64;

	int counter = 0;

	//int16_t bufferA[BUFFER_LENGTH];
	int16_t bufferB[BUFFER_LENGTH];

	//int16_t *frontBuffer = bufferA;
	int16_t *backBuffer = bufferB;

	while(true){

		//fill backbuffer
		memset(backBuffer, 0, BUFFER_LENGTH * sizeof(int16_t));

		for(int i = 0; i < sounds.size(); i++){

			Sound *sound_p = &sounds[i];

			for(int j = 0; j < BUFFER_LENGTH; j++){

				if(sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
					break;
				}

				backBuffer[j] += (int16_t)(
					MASTER_VOLUME *
					sound_p->volume *
					(float)sound_p->soundData_p->data[sound_p->n_playedSamples * sound_p->soundData_p->n_channels]
				);

				sound_p->n_playedSamples++;
			
			}

			if(sound_p->n_playedSamples >= sound_p->soundData_p->n_samples){
				sounds.erase(sounds.begin() + i);
				i--;
				continue;
			}
		
		}

		//write front buffer to sound card
		int error;
		pa_simple_write(s, backBuffer, BUFFER_LENGTH * sizeof(int16_t), &error);

		//switch buffers
		//int16_t *tmpPointer = backBuffer;
		//backBuffer = frontBuffer;
		//frontBuffer = tmpPointer;
		
	}

}

void Audio_setFrequency(float x){
	frequency = x;
}

#endif
