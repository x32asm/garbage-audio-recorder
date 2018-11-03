#ifndef GUARD_WAV_FORMAT_H
#define GUARD_WAV_FORMAT_H
#pragma once

#include <string>


void displayByteChars(const std::string&);

template <typename T>
std::string BigEndianIntToByteConversion(T num, int bytelen = sizeof(T)) {
	std::string bytes;

	for (int i = bytelen * 8 - 8; i >= 0; i -= 8) {
		if (i < sizeof(T) * 8) {
			bytes += (char)(((unsigned)num >> i) & 0xFF);
		}
		else bytes += (char)0;
	}

	return bytes;
}
template <typename T>
std::string LittleEndianIntToByteConversion(T num, int bytelen = sizeof(T)) {
	std::string bytes;
	unsigned char byte = 0;

	int mask, mshift = sizeof(T) * 8 - 1;
	int numshift = mshift;

	for (; mshift >= 0; --mshift) {
		mask = (T)1 << mshift;

		if (numshift > 0) byte |= (((unsigned)num << numshift) & mask) >> (mshift / 8 * 8);
		else byte |= (((unsigned)num >> -numshift) & mask) >> (mshift / 8 * 8);
		numshift -= 2;

		if (!(mshift % 8)) {
			bytes += byte; byte = 0;
		}
	}

	for (unsigned int i = 0; i < bytelen - sizeof(T); ++i) {
		bytes += (char)0;
	}

	return bytes;
}

#endif //GUARD_WAV_FORMAT_H