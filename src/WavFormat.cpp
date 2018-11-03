#include <iostream>

#include "WavFormat.h"


void displayByteChars(const std::string& bytechars) {
	for (char c : bytechars) {
		for (int i = sizeof(char) * 8 - 1; i >= 0; --i) {
			std::cout << (char)('0' + ((c >> i) & 1));
		}
		std::cout << ' ';
	}
	std::cout << '\n';
}
