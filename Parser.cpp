#include <windows.h>
#include <audioclient.h>

#include "Parser.h"


std::string parseKeyCode(int vkcode) {
	if (vkcode == VK_BACK) return "BACKSPACE";
	else if (vkcode == VK_TAB) return "TAB";
	else if (vkcode == VK_RETURN) return "ENTER";
	else if (vkcode == VK_LSHIFT || vkcode == VK_RSHIFT || vkcode == VK_SHIFT) return "SHIFT";
	else if (vkcode == VK_LCONTROL || vkcode == VK_RCONTROL || vkcode == VK_CONTROL) return "CONTROL";
	else if (vkcode == VK_LMENU || vkcode == VK_RMENU || vkcode == VK_MENU) return "ALT";
	else if (vkcode == VK_PAUSE) return "PAUSE";
	else if (vkcode == VK_CAPITAL) return "CAPS LOCK";
	else if (vkcode == VK_ESCAPE) return "ESCAPE";
	else if (vkcode == VK_SPACE) return "SPACE";
	else if (vkcode == VK_PRIOR) return "PAGE UP";
	else if (vkcode == VK_NEXT) return "PAGE DOWN";
	else if (vkcode == VK_END) return "END";
	else if (vkcode == VK_LEFT) return "LEFT";
	else if (vkcode == VK_RIGHT) return "RIGHT";
	else if (vkcode == VK_UP) return "UP ARROW";
	else if (vkcode == VK_DOWN) return "DOWN ARROW";
	else if (vkcode == VK_SELECT) return "SELECT";
	else if (vkcode == VK_PRINT) return "PRINT";
	else if (vkcode == VK_INSERT) return "INSERT";
	else if (vkcode == VK_HOME) return "HOME";
	else if (vkcode == VK_DELETE) return "DELETE";
	else if (vkcode == VK_HELP) return "HELP";
	else if (vkcode == VK_SLEEP) return "SLEEP";
	else if (vkcode == VK_MULTIPLY) return "MULTIPLY";
	else if (vkcode == VK_ADD) return "ADD";
	else if (vkcode == VK_SEPARATOR) return "SEPARATOR";
	else if (vkcode == VK_SUBTRACT) return "SUBTRACT";
	else if (vkcode == VK_DECIMAL) return "DECIMAL";
	else if (vkcode == VK_DIVIDE) return "DIVIDE";
	else if (vkcode == VK_NUMLOCK) return "NUM LOCK";
	else if (vkcode == VK_SCROLL) return "SCROLL LOCK";
	else if (vkcode == VK_RWIN || vkcode == VK_LWIN) return "WINDOWS";
	else if (vkcode == VK_APPS) return "APPS";
	else if (vkcode == VK_OEM_1) return ";:";
	else if (vkcode == VK_OEM_PLUS) return "+=";
	else if (vkcode == VK_OEM_COMMA) return ",<";
	else if (vkcode == VK_OEM_MINUS) return "-_";
	else if (vkcode == VK_OEM_PERIOD) return ".>";
	else if (vkcode == VK_OEM_2) return "/?";
	else if (vkcode == VK_OEM_3) return "`~";
	else if (vkcode == VK_OEM_4) return "[{";
	else if (vkcode == VK_OEM_5) return "\\|";
	else if (vkcode == VK_OEM_6) return "]}";
	else if (vkcode == VK_OEM_7) return "'\"";
	else if (vkcode >= 0x30 && vkcode <= 0x39) return std::to_string(vkcode - 0x30);
	else if (vkcode >= 0x41 && vkcode <= 0x5A) return std::string(1, (char)(vkcode - 0x41 + 'A'));
	else if (vkcode >= VK_NUMPAD0 && vkcode <= VK_NUMPAD9) return "NUMPAD " + std::string(1, (char)(vkcode - VK_NUMPAD0 + '0'));
	else if (vkcode >= VK_F1 && vkcode <= VK_F24) return "F" + std::to_string(vkcode - VK_F1 + 1);
	else {
		std::string ret;
		ret.resize(4);
		sprintf_s(&ret[0], 4, "%X", vkcode);
		return ret;
	}
}


std::string parseHResult(HRESULT hr) {
	if (hr == S_OK) return "S_OK";
	else if (hr == E_POINTER) return "E_POINTER";
	else if (hr == E_NOINTERFACE) return "E_NOINTERFACE";
	else if (hr == AUDCLNT_E_NOT_INITIALIZED) return "AUDCLNT_E_NOT_INITIALIZED";
	else if (hr == AUDCLNT_E_WRONG_ENDPOINT_TYPE) return "AUDCLNT_E_WRONG_ENDPOINT_TYPE";
	else if (hr == AUDCLNT_E_DEVICE_INVALIDATED) return "AUDCLNT_E_DEVICE_INVALIDATED";
	else if (hr == AUDCLNT_E_SERVICE_NOT_RUNNING) return "AUDCLNT_E_SERVICE_NOT_RUNNING";
	else return "DUNNO";
}