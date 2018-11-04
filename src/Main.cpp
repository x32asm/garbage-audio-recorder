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

std::size_t EFFECT_DESC_SIZE = 0, lastPrintLen = 0;
bool effectLastPrint = false;

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
			std::printf("\n%s", std::string(45, '\n').c_str());
			instructions(vkhome, vkstart, vkstop, vknext, vkprev, vkapply, vksave);
			effectLastPrint = false;
		}
		else if (vkcode != vkhome) {
			if (editstart && vkcode != vkstop) {
				vkstart = vkcode;
				editstart = false;
				SetConsoleTextAttribute(ctc::hConsole, ctc::keyColor);
				std::printf(" %s \n", parseKeyCode(vkcode).c_str());
				SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
				effectLastPrint = false;
			}
			else if (editstop && vkcode != vkstart) {
				vkstop = vkcode;
				editstop = false;
				SetConsoleTextAttribute(ctc::hConsole, ctc::keyColor);
				std::printf(" %s \n", parseKeyCode(vkcode).c_str());
				SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
				effectLastPrint = false;
			}
			else {
				if (vkcode == vkapply && recording) {
					sink.applyEffect(true);
				}
				else if (vkcode == vkstart && !recording) {
					recording = true;
					record = std::thread(rec::startRecording, &sink, &threcord);
					std::printf("\nstarted recording.\n");
					effectLastPrint = false;
				}
				else if (vkcode == vkstop && recording) {
					recording = false;
					rec::stopRecording(&threcord);
					record.join();
					millisRecorded = sink.msRecordLen();
					std::printf("\nstopped recording.\nrecorded %.4f seconds (%d ms)\n", millisRecorded / 1000., millisRecorded);
//					std::printf("enter a name for the wave file: ");
					std::string fname;
//					getline(std::cin, fname);
					millisRecorded = sink.saveAudioAsWav(saveDir, fname);
					std::printf("file saved as %s\n", fname.c_str());
					effectLastPrint = false;
				}
			}
		}
	}
	else if (wParam == WM_SYSKEYDOWN) {
		if (vkcode == vkstart) {
			editstart = true;
			SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
			std::printf("\nsetting start key:");
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			std::printf(" ");
			effectLastPrint = false;
		}
		else if (vkcode == vkstop) {
			editstop = true;
			SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
			std::printf("\nsetting stop key:");
			SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
			std::printf(" ");
			effectLastPrint = false;
		}
		else if (vkcode == vknext || vkcode == vkprev) {
			if (vkcode == vknext) {
				sink.nextEffect();
			}
			else if (vkcode == vkprev) {
				sink.prevEffect();
			}

			if (effectLastPrint) {
				std::cout << '\r';
			}
			else std::cout << '\n';

			int printlen;
			std::string desc;
			for (int i = -1; i < 2; ++i) {
				if (i == 0) SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
				else SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
				desc = effectDescriptions[(sink.getEffectIndex() + i >= 0) ? (sink.getEffectIndex() + i) % effectCount : effectCount + i];
				printlen += desc.size();
				std::printf("%s", desc.c_str());
				SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
				std::printf("%s", ((i < 1) ? " | " : ""));
			}
			if ((printlen += 6) < lastPrintLen) {
				for (std::size_t i = 0; i < lastPrintLen - printlen; ++i) {
					std::cout << ' ';
				}
			}
			lastPrintLen = printlen;

			effectLastPrint = true;
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

	for (int i = 0; i < effectCount; ++i) EFFECT_DESC_SIZE += effectDescriptions[i].size();

	HHOOK keybdhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookProc, NULL, 0);

	MessageLoop();

	exit(0);
}
