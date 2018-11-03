#ifndef GUARD_PARSER_H
#define GUARD_PARSER_H
#pragma once

#include <string>
#include <windows.h>

std::string parseKeyCode(int vkcode);
std::string parseHResult(HRESULT);

#endif //GUARD_PARSER_H
