#pragma once

#include "LuaParser/Define/LuaToken.h"
#include "LuaParser/File/LuaFile.h"
#include "LuaParser/Util/TextReader.h"
#include <map>
#include <stack>
#include <string_view>
#include <vector>

class LuaDocLexer {
public:
    enum class State {
        Init,
        ReadTag,
        TagClass,
        GenericDeclareList,

        TagField,
        TagAlias,

        TagGeneric,
        TagType,
        Type,
        TagParam,
        TagExpectedId,
        TagDiagnostic,

        ReadComment,
        ReadRest,
    };

    explicit LuaDocLexer(std::shared_ptr<LuaFile> file, std::string_view source, std::size_t offset);

    std::vector<LuaToken> &Tokenize();

private:
    static std::map<std::string, LuaTokenKind, std::less<>> LuaTag;

    static bool IsWhitespace(int ch);

    static bool IsIdStart(int ch);

    static bool IsIdContinue(int ch);

    LuaTokenKind Lex();

    LuaTokenKind ReadInit();

    LuaTokenKind ReadTag();

    LuaTokenKind ReadTagClass();

    LuaTokenKind ReadGenericDeclareList();

    LuaTokenKind ReadTagField();

    LuaTokenKind ReadTagAlias();

    LuaTokenKind ReadTagType();

    LuaTokenKind ReadTagGeneric();

    LuaTokenKind ReadTagParam();

    LuaTokenKind ReadTagExpectedId();

    LuaTokenKind ReadTagDiagnostic();

    LuaTokenKind ReadType();

    LuaTokenKind ReadCommentString();

    LuaTokenKind ReadNumeral();

    void TokenError(std::string_view message, TextRange range);

    void TokenError(std::string_view message, std::size_t offset);

    LuaTokenKind ReadId();

    LuaTokenKind ReadWhitespace();

    LuaTokenKind ReadRest();

    LuaTokenKind ReadString();

    void ExpectedType();

    void PushState(State state);

    void PopState();

    void ChangeState(State state);

    std::shared_ptr<LuaFile> _file;
    TextReader _reader;
    std::vector<LuaToken> _tokens;

    std::stack<State> _stateStack;
    std::size_t _typeLevel;
    bool _typeReq;
};
