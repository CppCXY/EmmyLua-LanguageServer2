#pragma once

#include "Lib/TextRange/TextRange.h"
#include "Lua/Kind/LuaTokenKind.h"
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
