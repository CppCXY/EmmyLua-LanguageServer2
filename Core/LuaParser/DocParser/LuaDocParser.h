#pragma once

#include <memory>
#include <vector>

#include "LuaParser/Define/LuaOperatorType.h"
#include "LuaParser/Define/LuaSyntaxError.h"
#include "LuaParser/Kind/LuaSyntaxNodeKind.h"
#include "LuaParser/Kind/LuaTokenKind.h"
#include "LuaParser/ParseState/ParseState.h"

class LuaFile;

class LuaDocParser {
public:
    LuaDocParser(const LuaFile *file, std::vector<LuaToken> &&tokens);

    bool Parse();

    std::vector<LuaToken> &GetTokens();

    ParseState &GetParseState();

    std::vector<LuaSyntaxError> &GetErrors();

private:
    void Next();

    LuaTokenKind LookAhead();

    LuaTokenKind Current();

    void Comments();

    CompleteMarker Comment();

    CompleteMarker EmmyLuaDoc();

    CompleteMarker DocClass();

    CompleteMarker DocInterface();

    CompleteMarker DocAlias();

    CompleteMarker DocField();

    CompleteMarker DocType();

    CompleteMarker DocParam();

    CompleteMarker DocContinue();

    CompleteMarker NormalComment();

    CompleteMarker GenericDefList();

    CompleteMarker TypeList();

    CompleteMarker Type();

    CompleteMarker PrimaryType();

    CompleteMarker TableType();

    CompleteMarker TableFieldType();

    CompleteMarker StringType();

    CompleteMarker FunctionType();

    CompleteMarker ParType();

    CompleteMarker ArrayType();

    void CheckAndNext(LuaTokenKind kind);

    bool TestAndNext(LuaTokenKind kind);

    const LuaFile *_file;
    ParseState _p;
    std::vector<LuaToken> _tokens;
    std::size_t _tokenIndex;
    bool _invalid;
    LuaTokenKind _current;
    std::vector<LuaSyntaxError> _errors;
};
