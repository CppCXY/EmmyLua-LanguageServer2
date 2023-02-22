#include "LuaDocLexer.h"

using enum LuaTokenKind;

LuaDocLexer::LuaDocLexer(std::shared_ptr<LuaFile> file, std::string_view source, std::size_t offset)
    : _file(file),
      _reader(source, offset),
      _state(State::Init),
      _typeLevel(0),
      _typeReq(false) {
}

std::vector<LuaToken> &LuaDocLexer::Tokenize() {
    return _tokens;
}

LuaTokenKind LuaDocLexer::Lex() {
    if (_reader.IsEof()) {
        return TK_EOF;
    }
    switch (_state) {
        case State::Init: {
            return ReadInit();
        }
        case State::ReadTag:
            break;
        case State::ClassID:
            break;
        case State::InterfaceID:
            break;
        case State::GenericDeclareList:
            break;
        case State::FieldID:
            break;
        case State::AliasID:
            break;
        case State::Generic:
            break;
        case State::Type:
            break;
        case State::ParamId:
            break;
        case State::SeeId:
            break;
        case State::LanguageId:
            break;
        case State::DiagnosticAction:
            break;
        case State::DiagnosticIDList:
            break;
        case State::ReadComment:
            break;
        case State::ReadRest:
            break;
        default: {
            return TK_EOF;
        }
    }
}
LuaTokenKind LuaDocLexer::ReadInit() {
    switch (_reader.GetCurrentChar()) {
        case '-': {
            auto count = _reader.EatWhen('-');
            if (count == 3) {
                return TK_DASH_DASH_DASH;
            } else {
                _state = State::ReadComment;
                return TK_DASHES;
            }
        }
        case '@': {
            _state = State::ReadTag;
            return TK_AT;
        }
        default: {
            if (std::isspace(_reader.GetCurrentChar())) {
            }
        }
    }
}
