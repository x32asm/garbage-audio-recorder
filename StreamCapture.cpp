#include <iostream>
#include <thread>
#include <string>

#include "StreamCapture.h"
#include "AudioSink.h"
#include "Parser.h"


namespace id {
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
	const IID IID_IAudioClient = __uuidof(IAudioClient);
	const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);
}


StreamCapture::StreamCapture() :
	cont(false) {
	
}


void thRecord(AudioSink*, bool*);
void rec::startRecording(AudioSink* sink, bool* cont) {
	thRecord(sink, &(*cont = true));
}
void rec::stopRecording(bool* cont) {
	*cont = false;
}



#define SAFE_RELEASE(punk) if (punk != NULL) {punk->Release(); punk = NULL;}
void thRecord(AudioSink* pSink, bool* pCont) {
	HRESULT hr = S_OK;
	REFERENCE_TIME hnsRequestedDuration = StreamCapture::REFTIMES_PER_SEC;
	REFERENCE_TIME hnsActualDuration;
	UINT32 bufferFrameCount;
	UINT32 numFramesAvailable;
	IMMDeviceEnumerator* pEnumerator = NULL;
	IMMDevice* pDevice = NULL;
	IAudioClient* pAudioClient = NULL;
	IAudioCaptureClient* pCaptureClient = NULL;
	WAVEFORMATEX* pwfx = NULL;
	UINT32 packetLength = 0;
	BYTE* pData;
	DWORD flags;

	CoInitialize(NULL);
	hr = CoCreateInstance(id::CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, id::IID_IMMDeviceEnumerator, (void**)&pEnumerator);
	if (FAILED(hr)) {
		goto Exit;
	}

	hr = pEnumerator->GetDefaultAudioEndpoint(eCapture, eConsole, &pDevice);
	if (FAILED(hr)) {
		goto Exit;
	}

	hr = pDevice->Activate(id::IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient);
	if (FAILED(hr)) {
		goto Exit;
	}

	hr = pAudioClient->GetMixFormat(&pwfx);
	if (FAILED(hr)) {
		goto Exit;
	}
	pSink->clear();
	pSink->setFormat(pwfx);

	//initialize audio client
	hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, hnsRequestedDuration, 0, pwfx, NULL);

	//size of allocated buffer
	hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	if (FAILED(hr)) {
		goto Exit;
	}

	//get capture client
	hr = pAudioClient->GetService(id::IID_IAudioCaptureClient, (void**)&pCaptureClient);
	if (FAILED(hr)) {
		goto Exit;
	}

	//actual duration of the buffer
	hnsActualDuration = (REFERENCE_TIME)((double)StreamCapture::REFTIMES_PER_SEC * bufferFrameCount / pwfx->nSamplesPerSec);

	hr = pAudioClient->Start();
	if (FAILED(hr)) {
		goto Exit;
	}

	while (*pCont) {
		Sleep((DWORD)(hnsActualDuration / StreamCapture::REFTIMES_PER_MILLISEC / 2));
		hr = pCaptureClient->GetNextPacketSize(&packetLength);
		if (FAILED(hr)) {
			goto Exit;
		}

		while (packetLength != 0) {
			hr = pCaptureClient->GetBuffer(&pData, &numFramesAvailable, &flags, NULL, NULL);
			if (FAILED(hr)) {
				goto Exit;
			}

			hr = pSink->copyData(pData, numFramesAvailable);
			if (FAILED(hr)) {
				goto Exit;
			}

			hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
			if (FAILED(hr)) {
				goto Exit;
			}
			hr = pCaptureClient->GetNextPacketSize(&packetLength);
			if (FAILED(hr)) {
				goto Exit;
			}
		}
	}

	hr = pAudioClient->Stop();
	if (FAILED(hr)) {
		goto Exit;
	}

	return;

Exit:
	std::cout << parseHResult(hr) << '\n';
	CoTaskMemFree(pwfx);
	SAFE_RELEASE(pEnumerator);
	SAFE_RELEASE(pDevice);
	SAFE_RELEASE(pAudioClient);
	SAFE_RELEASE(pCaptureClient);
}