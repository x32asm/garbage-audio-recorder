#ifndef GUARD_COLORCONSOLE_H
#define GUARD_COLORCONSOLE_H
#pragma once

#include <Windows.h>

namespace {
	namespace ctc {
		enum TextColor {
			BLACK, DARK_BLUE, DARK_GREEN, DARK_TURQOISE, DARK_RED, DARK_MAGENTA, DARK_YELLOW, LIGHT_GRAY, GRAY, BLUE, GREEN, TURQOISE, RED, MAGENTA, YELLOW, WHITE
		};

		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int defaultColor = LIGHT_GRAY;
		int bannerColor = GRAY;
		int titleColor = GRAY + DARK_RED * 16;
		int authorColor = RED;
		int indicatorColor = BLACK + LIGHT_GRAY * 16;
		int keyColor = DARK_RED + LIGHT_GRAY * 16;

		int errColor = DARK_RED + GRAY * 16;
		int errMessageColor = WHITE + GRAY * 16;
	}
}


std::string promptDirectory();
void instructions(int, int, int, int, int, int);
void startMessage(int, int, int, int, int, int);

#endif //GUARD_COLORCONSOLE_H