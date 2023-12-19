#ifndef WAV_READER_H_
#define WAV_READER_H_

#include "stdint.h"
#include "stddef.h"

//get data and number of pcm frames
int16_t *WavReader_getDataFromWavFile(char *, int *);

#endif
