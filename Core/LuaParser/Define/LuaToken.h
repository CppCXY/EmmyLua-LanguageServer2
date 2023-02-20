#pragma once

#include "Lib/TextRange/TextRange.h"
#include "LuaParser/Kind/LuaTokenKind.h"
#include <string_view>

class LuaToken
{
public:
	LuaToken(LuaTokenKind type, TextRange range)
		: TokenType(type),
		  Range(range)
	{
	}

	LuaTokenKind TokenType;
	TextRange Range;
};
