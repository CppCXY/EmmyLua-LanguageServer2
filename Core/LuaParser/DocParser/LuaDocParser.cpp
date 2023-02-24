#include "LuaDocParser.h"
#include "LuaParser/File/LuaFile.h"
#include "LuaParser/exception/LuaParseException.h"

using enum LuaTokenKind;

#define TryElseReturn(call) if(call.IsNone()) { return CompleteMarker(); }

LuaDocParser::LuaDocParser(const LuaFile *file, std::vector<LuaToken> &&tokens)
    : _file(file),
      _tokens(tokens),
      _tokenIndex(0),
      _invalid(true),
      _current(TK_EOF) {
}

bool LuaDocParser::Parse() {
    Comments();

    return true;
}

std::vector<LuaToken> &LuaDocParser::GetTokens() {
    return _tokens;
}

ParseState &LuaDocParser::GetParseState() {
    return _p;
}

std::vector<LuaSyntaxError> &LuaDocParser::GetErrors() {
    return _errors;
}

void LuaDocParser::Next() {
    auto tk = Current();
    auto me = MarkEvent(MarkEventType::EatToken);
    me.U.Token.Kind = tk;
    me.U.Token.Index = _tokenIndex;
    _p.GetEvents().push_back(me);
    _tokenIndex++;
    _invalid = true;
}

LuaTokenKind LuaDocParser::LookAhead() {
    std::size_t nextIndex = _tokenIndex + 1;

    if (nextIndex < _tokens.size()) {
        return _tokens[nextIndex].TokenType;
    }

    return TK_EOF;
}

LuaTokenKind LuaDocParser::Current() {
    if (_invalid) {
        _invalid = false;
        if (_tokenIndex < _tokens.size()) {
            _current = _tokens[_tokenIndex].TokenType;
        } else {
            _current = TK_EOF;
        }
    }

    return _current;
}

void LuaDocParser::Comments() {
    while (Current() != TK_EOF) {
        Comment();
    }
}

CompleteMarker LuaDocParser::Comment() {
    if (Current() == TK_DASH_DASH_DASH) {
        Next();
        if (Current() == TK_AT) {
            Next();
            return EmmyLuaDoc();
        }
    }
    return NormalComment();
}

CompleteMarker LuaDocParser::EmmyLuaDoc() {
    switch (Current()) {
        case TK_TAG_CLASS: {
            return DocClass();
        }
        case TK_TAG_INTERFACE: {
            return DocInterface();
        }
        default: {
            return CompleteMarker();
        }
    }
}

void LuaDocParser::CheckAndNext(LuaTokenKind kind) {
    if (Current() == kind) {
        Next();
    }
}

bool LuaDocParser::TestAndNext(LuaTokenKind kind) {
    if (Current() == kind) {
        Next();
        return true;
    }
    return false;
}

CompleteMarker LuaDocParser::DocClass() {
    auto m = _p.Mark();
    // class
    Next();

    CheckAndNext(TK_NAME);

    if (Current() == TK_LT) {
        GenericDefList();
    }

    if (Current() == TK_COLON) {
        Next();
        TypeList();
    }

    TestAndNext(TK_AT);

    TestAndNext(TK_COMMENT_CONTENT);

    return m.Complete(_p, LuaSyntaxNodeKind::DocClass);
}

CompleteMarker LuaDocParser::GenericDefList() {
    auto m = _p.Mark();

    CheckAndNext(TK_LT);

    do {
        CheckAndNext(TK_NAME);
    } while (TestAndNext(TK_COMMA));

    CheckAndNext(TK_GT);

    return m.Complete(_p, LuaSyntaxNodeKind::GenericDefList);
}

CompleteMarker LuaDocParser::TypeList() {
    auto m = _p.Mark();

    do {
        if (Type().IsNone()) {
            break;
        }
    } while (TestAndNext(TK_COMMA));

    return m.Complete(_p, LuaSyntaxNodeKind::TypeList);
}

CompleteMarker LuaDocParser::Type() {
    auto m = _p.Mark();


    return m.Complete(_p, LuaSyntaxNodeKind::Type);
}

CompleteMarker LuaDocParser::PrimaryType() {
    switch (Current()) {
        case TK_LCURLY: {
            return TableType();
        }
        case TK_NAME: {
        }
        case TK_STRING: {
        }
        case TK_FUN: {
        }
        case TK_LPARN: {
        }
        default: {
            return CompleteMarker();
        }
    }
}

CompleteMarker LuaDocParser::TableType() {
    auto m = _p.Mark();

    CheckAndNext(TK_LCURLY);

    while (TableFieldType().IsComplete()) {
        switch (Current()) {
            case TK_COMMA:
            case TK_SEMI: {
                Next();
                break;
            }
            default: {
                goto endLoop;
            }
        }
    }
endLoop:

    CheckAndNext(TK_RCURLY);

    return m.Complete(_p, LuaSyntaxNodeKind::TableType);
}

// { aaaa: number, [1]: number, [string]: number }
CompleteMarker LuaDocParser::TableFieldType() {
    auto m = _p.Mark();

    if (Current() == TK_NAME) {
        Next();
    } else if (Current() == TK_LBRACKET) {
        Next();
        Type();
    }
    CheckAndNext(TK_COLON);
    Type();

    return m.Complete(_p, LuaSyntaxNodeKind::TableFieldType);
}
