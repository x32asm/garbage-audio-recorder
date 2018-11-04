#include <windows.h>
#include <windowsx.h>
#include <mmdeviceapi.h>
#include <string>
#include <cstdio>
#include <thread>
#include <iostream>

#include "UserInterface.h"
#include "Parser.h"
#include "StreamCapture.h"


int vkedit = VK_MENU, vkhome = VK_HOME, vksave = VK_HOME, vkstart = VK_F9, vkstop = VK_F10, vknext = VK_RIGHT, vkprev = VK_LEFT, vkapply = VK_END;

bool alt = false;
bool editstart = false, editstop = false;

bool threcord = false;
bool recording = false;
bool startedThread = false;
int millisRecorded = 0;

AudioSink sink;
std::string saveDir;

std::thread record;

LRESULT CALLBACK KeyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	KBDLLHOOKSTRUCT* keybdInfo = (KBDLLHOOKSTRUCT*)lParam;
	DWORD vkcode = keybdInfo->vkCode;
	if (wParam == WM_KEYDOWN) {
		if (vkcode == vkhome && !editstart && !editstop) {
			std::printf("%s", std::string(45, '\n').c_str());
			instructions(vkhome, vkstart, vkstop, vknext, vkprev, vkapply, vksave);
		}
		else if (vkcode != vkhome) {
			if (editstart && vkcode != vkstop) {
				vkstart = vkcode;
				editstart = false;
				SetConsoleTextAttribute(ctc::hConsole, ctc::keyColor);
				std::printf(" %s \n", parseKeyCode(vkcode).c_str());
				SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			}
			else if (editstop && vkcode != vkstart) {
				vkstop = vkcode;
				editstop = false;
				SetConsoleTextAttribute(ctc::hConsole, ctc::keyColor);
				std::printf(" %s \n", parseKeyCode(vkcode).c_str());
				SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			}
			else {
				if (vkcode == vkapply && recording) {
					sink.applyEffect(true);
				}
				else if (vkcode == vkstart && !recording) {
					recording = true;
					record = std::thread(rec::startRecording, &sink, &threcord);
					std::printf("started recording.\n");
				}
				else if (vkcode == vkstop && recording) {
					recording = false;
					rec::stopRecording(&threcord);
					record.join();
					millisRecorded = sink.msRecordLen();
					std::printf("stopped recording.\nrecorded %.4f seconds (%d ms)\n", millisRecorded / 1000., millisRecorded);
//					std::printf("enter a name for the wave file: ");
					std::string fname;
//					getline(std::cin, fname);
					millisRecorded = sink.saveAudioAsWav(saveDir, fname);
					std::printf("file saved as %s\n", fname.c_str());
				}
			}
		}
	}
	else if (wParam == WM_SYSKEYDOWN) {
		if (vkcode == vkstart) {
			editstart = true;
			SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
			std::printf("setting start key:");
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			std::printf(" ");
		}
		else if (vkcode == vkstop) {
			editstop = true;
			SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
			std::printf("setting stop key:");
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			std::printf(" ");
		}
		else if (vkcode == vknext) {
			sink.nextEffect();
			SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
			std::printf("effect:");
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			std::printf(" ");
			std::printf("%s\n", sink.getEffect().c_str());
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
		}
		else if (vkcode == vkprev) {
			sink.prevEffect();
			SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
			std::printf("effect:");
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			std::printf(" ");
			std::printf("%s\n", sink.getEffect().c_str());
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
		}
		else if (vkcode == vksave) {
//			setDirectory(true);
		}
	}
	else if (wParam == WM_KEYUP) {
		if (vkcode == vkapply && recording) {
			sink.applyEffect(false);
		}
	}

	return CallNextHookEx(0, nCode, wParam, lParam);
}

void MessageLoop() {
	MSG msg;
	while (1) {
		if (GetMessage(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(100);
	}
}

int main(int argc, char** argv) {
	startMessage(vkhome, vkstart, vkstop, vknext, vkprev, vkapply, vksave);
	saveDir = setDirectory(true);

	HHOOK keybdhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);

	MessageLoop();

	exit(0);
}
