#pragma once

#include <map>
#include <string_view>
#include <memory>
#include <string>
#include <vector>
#include <set>

#include "LuaParser/Define/LuaSyntaxError.h"
#include "LuaParser/Define/LuaToken.h"
#include "LuaParser/Kind/LuaTokenKind.h"
#include "LuaParser/Util/TextReader.h"
#include "LuaParser/File/LuaFile.h"

/*
 * token 解析来自于lua 源代码,实现上非常接近但细节处并不相同
 */
class LuaLexer
{
public:
	explicit LuaLexer(std::shared_ptr<LuaFile> file);

    std::vector<LuaToken>& Tokenize();
private:
	static std::map<std::string, LuaTokenKind, std::less<>> LuaReserved;

	LuaTokenKind Lex();

	LuaTokenKind ReadNumeral();

	std::size_t SkipSep();

	void ReadLongString(std::size_t sep);

	void ReadString(int del);

	void IncLinenumber();

	bool CurrentIsNewLine();

	bool IsReserved(std::string_view text);

	void TokenError(std::string_view message, TextRange range);

    void TokenError(std::string_view message, std::size_t offset);

    std::shared_ptr<LuaFile> _file;
	int _linenumber;
    TextReader _reader;
	std::vector<LuaToken> _tokens;
};
