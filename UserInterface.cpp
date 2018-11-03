#include <string>
#include <cstdio>
#include <iostream>
#include <lmcons.h>

#include "UserInterface.h"
#include "Parser.h"


int biggest(int a, int b) {
	return (a > b) ? a : b;
}


std::string getUsername() {
	std::string usr; usr.resize(UNLEN + 1);
	DWORD len;
	GetUserNameA(&usr[0], &len);
	usr.resize(len - 1);

	return usr;
}
std::string promptDirectory() {
	SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
	std::printf("enter the save directory (leave empty for desktop): ");
	std::string directory;
	getline(std::cin, directory);

	if (directory == "") {
		directory = "C:/Users/" + getUsername() + "/Desktop/";
	}
	else if (directory[directory.size() - 1] != '/' && directory.find('/') != -1) {
		std::cout << "or here\n";
		directory += '/';
	}
	else if (directory[directory.size() - 1] != '\\' && directory.find('\\') != -1) {
		directory += '\\';
	}
	std::printf("all files will be saved to %s\n", directory.c_str());

	return directory;
}

void printmsg(const std::string& vk, const std::string msg, int lmsglen) {
	SetConsoleTextAttribute(ctc::hConsole, ctc::errColor);
	if (vk.size() < 8) std::printf("%s\t\t", vk.c_str());
	else std::printf("%s\t", vk.c_str());
	SetConsoleTextAttribute(ctc::hConsole, ctc::errMessageColor);
	std::printf("%s%s\n", msg.c_str(), std::string(lmsglen - msg.size(), ' ').c_str());
}
void instructions(int vkhome, int vkstart, int vkstop, int vknext, int vkprev, int vkapply) {
	std::string homevk = parseKeyCode(vkhome);
	std::string startvk = parseKeyCode(vkstart), stopvk = parseKeyCode(vkstop);
	std::string applyvk = parseKeyCode(vkapply);
	std::string nextvk = "ALT + " + parseKeyCode(vknext), prevvk = "ALT + " + parseKeyCode(vkprev);
	static std::string homemsg = "displays instructions";
	static std::string startmsg = "starts recording", stopmsg = "stops recording";
	static std::string nextmsg = "selects the next effect", prevmsg = "selects the previous effect";
	static std::string applymsg = "applies the selected effect to the audio while held";
	static std::string editmsg = "press ALT + the current start/stop key to re-map it to a different key\nNOTE: start and stop keys cannot be the same";
	static int lmsglen = applymsg.size();
	SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
	std::printf(" INSTRUCTIONS \n");
	printmsg(homevk, homemsg, lmsglen);
	printmsg(startvk, startmsg, lmsglen);
	printmsg(stopvk, stopmsg, lmsglen);
	printmsg(nextvk, nextmsg, lmsglen);
	printmsg(prevvk, prevmsg, lmsglen);
	printmsg(applyvk, applymsg, lmsglen);
	SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
	std::printf("%s\n\n\n", editmsg.c_str());

}
void startMessage(int vkhome, int vkstart, int vkstop, int vknext, int vkprev, int vkapply) {
	const static int dashamnt = 10;
	std::string title = "garbage audio recording software";
	std::string mby = "made by: ", author = "x32asm";
	std::string dashes(dashamnt, '-');
	std::string line((dashes.size() + 3) * 2 + title.size(), '_');

	SetConsoleTextAttribute(ctc::hConsole, ctc::bannerColor);
	std::printf("%s\n\n> %s ", line.c_str(), dashes.c_str());
	SetConsoleTextAttribute(ctc::hConsole, ctc::titleColor);
	std::printf("%s", title.c_str());
	SetConsoleTextAttribute(ctc::hConsole, ctc::bannerColor);
	std::printf(" %s <\n%s\n%s", dashes.c_str(), line.c_str(), std::string((line.size() - (author.size() + mby.size())) / 2, ' ').c_str());

	SetConsoleTextAttribute(ctc::hConsole, ctc::bannerColor);
	std::printf("%s", mby.c_str());
	SetConsoleTextAttribute(ctc::hConsole, ctc::authorColor);
	std::printf("%s\n\n", author.c_str());

	instructions(vkhome, vkstart, vkstop, vknext, vkprev, vkapply);

	SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
}