#pragma once

#include "Core/LuaParser/Define/LuaTokenKind.h"
#include "TextRange.h"
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
