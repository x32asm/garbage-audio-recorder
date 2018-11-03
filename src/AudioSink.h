#ifndef GUARD_AUDIO_SINK_H
#define GUARD_AUDIO_SINK_H
#pragma once

#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

const static std::string effectDescriptions[] = { "No effect", "High gain", "Low frequency; High gain", "Pitch shifted (up)", "Pitch shifted (down)", "Normalized", "Distorted amplitude", "Distorted pitch" };
const static std::size_t effectCount = 8;

class AudioSink {
public:
	AudioSink();

	void clear();

	int saveAudioAsWav(const std::string&, std::string&);
	void setFormat(WAVEFORMATEX*);
	HRESULT copyData(BYTE*, UINT32);

	int msRecordLen() { return 1000 * totalFrames / pwfx->nSamplesPerSec; }
	std::string getEffect() { return effectDescriptions[effectIndex]; }
	int getEffectIndex() { return effectIndex; }
	void nextEffect() { ++effectIndex %= effectCount; }
	void prevEffect() {
		if (effectIndex == 0) effectIndex = effectCount - 1;
		else --effectIndex;
	}
	void applyEffect(bool b) { eapply = b; }

private:
	template <typename dt>
	void convertType(dt* arr) {
		if (sizeof(arr) < sizeof(dt)) return;
		const unsigned int dt_size = sizeof(dt);
		dt d;
		BYTE b[dt_size];
		std::size_t arrind = 0;
		for (unsigned int i = 0; i < audioData.size(); ++i) {
			b[i % dt_size] = audioData[i];
			if ((i + 1) % dt_size == 0 && i != 0) {
				memcpy(&d, b, sizeof(dt));
				arr[arrind++] = d;
			}
		}
	}

	unsigned char effectIndex;
	bool eapply;

	WAVEFORMATEX* pwfx;
	unsigned int pcmFormat;
	std::vector<BYTE> audioData;
	UINT32 totalFrames;
};

#endif //GUARD_AUDIO_SINK_H
