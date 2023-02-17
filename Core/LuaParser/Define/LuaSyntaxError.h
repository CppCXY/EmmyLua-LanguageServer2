#pragma once

#include "Core/LuaParser/Define/LuaTokenKind.h"
#include "TextRange.h"
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
