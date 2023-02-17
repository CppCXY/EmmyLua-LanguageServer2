#pragma once

#include <map>
#include <string_view>
#include <memory>
#include <string>
#include <vector>
#include <set>

#include "Core/LuaParser/Define/LuaSyntaxError.h"
#include "Core/LuaParser/Define/LuaToken.h"
#include "Core/LuaParser/Define/LuaTokenKind.h"
#include "Core/LuaParser/Util/TextReader.h"
#include "LuaParser/File/LuaFile.h"

/*
 * token 解析来自于lua 源代码,实现上非常接近但细节处并不相同
 */
class LuaLexer
{
public:
	explicit LuaLexer(std::shared_ptr<LuaFile> file);

    std::vector<LuaToken>& Tokenize();

	std::vector<LuaSyntaxError>& GetErrors();

	bool HasError() const;

	std::shared_ptr<LuaFile> GetFile();
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

	int _linenumber;
    bool _supportNonStandardSymbol;
    TextReader _reader;
	std::vector<LuaToken> _tokens;
	std::vector<LuaSyntaxError> _errors;
	std::shared_ptr<LuaFile> _file;
};
