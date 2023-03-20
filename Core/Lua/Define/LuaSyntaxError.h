#pragma once

#include "Lib/TextRange/TextRange.h"
#include "Lua/Kind/LuaTokenKind.h"
#include <string>

class LuaSyntaxError {
public:
    LuaSyntaxError(std::string_view errorMessage, TextRange range)
		: Message(errorMessage),
          Range(range)
	{
	}

	std::string Message;
	TextRange Range;
};
