#pragma once

#include "LuaParser/Define/LuaToken.h"
#include "LuaParser/File/LuaFile.h"
#include "LuaParser/Util/TextReader.h"
#include <map>
#include <string_view>
#include <vector>

class LuaDocLexer {
public:
    enum class State {
        Init,
        ReadTag,
        ClassID,
        InterfaceID,
        GenericDeclareList,

        FieldID,
        AliasID,

        Generic,
        Type,
        ParamId,
        SeeId,
        LanguageId,
        DiagnosticAction,
        DiagnosticIDList,

        ReadComment,
        ReadRest,
    };

    explicit LuaDocLexer(std::shared_ptr<LuaFile> file, std::string_view source, std::size_t offset);

    std::vector<LuaToken> &Tokenize();

private:
    static std::map<std::string, LuaTokenKind, std::less<>> LuaTag;

    LuaTokenKind Lex();

    LuaTokenKind ReadInit();



    LuaTokenKind ReadNumeral();

    void ReadString(int del);

    bool IsReserved(std::string_view text);

    void TokenError(std::string_view message, TextRange range);

    void TokenError(std::string_view message, std::size_t offset);

    std::shared_ptr<LuaFile> _file;
    TextReader _reader;
    std::vector<LuaToken> _tokens;
    State _state;
    std::size_t _typeLevel;
    bool _typeReq;
};
