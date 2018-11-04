#include <string>
#include <cstdio>
#include <iostream>
#include <lmcons.h>

#include "UserInterface.h"
#include "Parser.h"


std::string getUsername() {
	std::string usr; usr.resize(UNLEN + 1);
	DWORD len;
	GetUserNameA(&usr[0], &len);
	usr.resize(len - 1);

	return usr;
}
bool dirExists(const std::string& dirName_in) {
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		std::printf("The provided directory does not exist.\n");
		return false;  //invalid path

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;   //correct path

	return false;    //path leads to a file
}
std::string setDirectory(bool prompt) {
	SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
	if (prompt) std::printf("enter the save directory (leave empty for desktop): ");
	std::string directory;
	if (prompt) getline(std::cin, directory);

	if (directory == "" || !dirExists(directory)) {
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
void instructions(int vkhome, int vkstart, int vkstop, int vknext, int vkprev, int vkapply, int vksave) {
	std::string homevk = parseKeyCode(vkhome);
	std::string savevk = "ALT + " + parseKeyCode(vksave);
	std::string startvk = parseKeyCode(vkstart), stopvk = parseKeyCode(vkstop);
	std::string applyvk = parseKeyCode(vkapply);
	std::string nextvk = "ALT + " + parseKeyCode(vknext), prevvk = "ALT + " + parseKeyCode(vkprev);
	static std::string homemsg = "displays instructions";
	static std::string savemsg = "prompts user for a new save directory";
	static std::string startmsg = "starts recording", stopmsg = "stops recording";
	static std::string nextmsg = "selects the next effect", prevmsg = "selects the previous effect";
	static std::string applymsg = "applies the selected effect to the audio while held";
	static std::string editmsg = "press ALT + the current start/stop key to re-map it to a different key\nNOTE: start and stop keys cannot be the same";
	static int lmsglen = applymsg.size();
	SetConsoleTextAttribute(ctc::hConsole, ctc::indicatorColor);
	std::printf(" INSTRUCTIONS \n");
	printmsg(homevk, homemsg, lmsglen);
//	printmsg(savevk, savemsg, lmsglen);
	printmsg(startvk, startmsg, lmsglen);
	printmsg(stopvk, stopmsg, lmsglen);
	printmsg(nextvk, nextmsg, lmsglen);
	printmsg(prevvk, prevmsg, lmsglen);
	printmsg(applyvk, applymsg, lmsglen);
	SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
	std::printf("%s\n\n", editmsg.c_str());

}
void startMessage(int vkhome, int vkstart, int vkstop, int vknext, int vkprev, int vkapply, int vksave) {
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

	instructions(vkhome, vkstart, vkstop, vknext, vkprev, vkapply, vksave);

	SetConsoleTextAttribute(ctc::hConsole, ctc::defaultColor);
}
