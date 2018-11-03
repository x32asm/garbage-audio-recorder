#ifndef GUARD_AUDIO_RECORDER_H
#define GUARD_AUDIO_RECORDER_H
#pragma once

#include <mmdeviceapi.h>
#include <audioclient.h>

#include "AudioSink.h"

class StreamCapture {
public:
	const static int REFTIMES_PER_SEC = 1000;
	const static int REFTIMES_PER_MILLISEC = 1;

	StreamCapture();

	bool cont;
};

namespace rec {
	void startRecording(AudioSink*, bool*);
	void stopRecording(bool*);
}

#endif //GUARD_AUDIO_RECORDER_H