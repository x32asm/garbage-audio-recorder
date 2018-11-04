#include <iostream>
#include <cstdio>
#include <fstream>
#include <chrono>
#include <algorithm>

#include "AudioSink.h"
#include "sndfile.hh"


AudioSink::AudioSink() :
	effectIndex(0), eapply(0), pwfx(NULL), pcmFormat(), audioData(), totalFrames(0) {
}


void AudioSink::clear() {
	pwfx = NULL;
	pcmFormat = NULL;
	audioData.clear();
	totalFrames = 0;
}


std::string getTime() {
	return std::to_string(static_cast<int>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())));
}
int AudioSink::saveAudioAsWav(const std::string& dir, std::string& fname) {
	if (fname == "") fname = "audio-" + getTime() + ".wav";
	else if (fname.size() < 5 || fname.substr(fname.size() - 4) != ".wav") {
		fname += ".wav";
	}
	SndfileHandle outfile((dir + fname).c_str(), SFM_WRITE, SF_FORMAT_WAV | pcmFormat, pwfx->nChannels, pwfx->nSamplesPerSec);
	if (effectIndex == 2) {
		short* data = new short[audioData.size() / sizeof(short)];
		convertType(data);
		outfile.write(data, totalFrames * pwfx->nChannels);
		delete[] data;
	}
	else {
		float* data = new float[audioData.size() / sizeof(float)];
		convertType(data);
		outfile.write(data, totalFrames * pwfx->nChannels);
		delete[] data;
	}
	return msRecordLen();
}

void AudioSink::setFormat(WAVEFORMATEX* pwfx) {
	AudioSink::pwfx = pwfx;
//	std::printf("nChannels: %d\nnSamplesPerSec: %d\nnAvgBytesPerSec: %d\nnBlockAlign: %d\nwBitsPerSample: %d\n", pwfx->nChannels, pwfx->nSamplesPerSec, pwfx->nAvgBytesPerSec, pwfx->nBlockAlign, pwfx->wBitsPerSample);
	pcmFormat = (pwfx->wBitsPerSample == 32) ? SF_FORMAT_PCM_32 : (pwfx->wBitsPerSample) ? SF_FORMAT_PCM_24 : (pwfx->wBitsPerSample == 16) ? SF_FORMAT_PCM_16 : SF_FORMAT_PCM_U8;
}

HRESULT AudioSink::copyData(BYTE* pData, UINT32 numFrames) {
	srand((unsigned)time(NULL));
	unsigned char effectIndex = this->effectIndex;
	unsigned int dt_size = (effectIndex == 1) ? sizeof(int) : (effectIndex == 2) ? sizeof(short) : sizeof(float);
	float gain = 5;
	float fsample;
	double dsample;
	BYTE* b = new BYTE[dt_size];

	int temp = 0;
	for (unsigned int i = 0; i < numFrames * pwfx->nChannels * dt_size; ++i) {
		b[i % dt_size] = pData[i];
		if ((i + 1) % dt_size == 0 && i != 0) {
			if (effectIndex != 2) {
				memcpy(&fsample, b, min(dt_size, sizeof(float)));
				if (eapply) {
					switch (effectIndex) {
					case 1:
						gain = 1;
						fsample += (int)(fsample + 1)*1.5f;
						break;
					case 3:
						//pitch up
						break;
					case 4:
						//pitch down
						break;
					case 5:
						gain = 8;
						fsample = abs(fsample);
						//disturbed
						break;
					case 6:
						gain = 1;
						if (fsample > 0.01) fsample += 0.1f;
						else if (fsample > 0.001) fsample = fsample * 10 + 0.01f;
						else if (fsample < -0.01) fsample = -0.1f;
						else if (fsample < -0.001) fsample *= 100;
						else fsample -= 0.1f;
						//walkie-talkie
						break;
					case 7:
						//anonymous
						break;
					}
				}
				dsample = (double)fsample * gain;
				if (dsample>32767.0) { dsample = 32767.0; }
				if (dsample<-32768.0) { dsample = -32768.0; }
				fsample = (float)dsample;
				memcpy(b, &fsample, min(dt_size, sizeof(float)));
				for (UINT8 d = 0; d < dt_size; ++d) audioData.push_back(b[d]);
			}
			else {
				for (UINT8 d = 0; d < dt_size; ++d) audioData.push_back(b[d]);
				//demonic
			}
		}
	}

	totalFrames += numFrames;
	delete[] b;

	return S_OK;
}
