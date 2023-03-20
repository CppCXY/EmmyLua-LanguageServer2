#include "TreeUpdateEvent.h"
#include "Lua/Kind/LuaTokenKind.h"
#include "Lua/LuaLexer/LuaLexer.h"
#include "LuaSyntaxTree.h"

using enum LuaTokenKind;

TreeUpdateEvent TreeUpdateEvent::From(SourceUpdateEvent &sourceUpdateEvent, LuaSyntaxTree &t) {
    TreeUpdateEvent e;
    e.SourceEvent = sourceUpdateEvent;
    switch (sourceUpdateEvent.UpdateAction) {
        case SourceUpdateEvent::Action::Add: {
            e.InitByAdd(sourceUpdateEvent, t);
            break;
        }
        case SourceUpdateEvent::Action::Delete: {
            break;
        }
        case SourceUpdateEvent::Action::Replace: {
            break;
        }
        case SourceUpdateEvent::Action::None: {
            break;
        }
    }

    return e;
}

TreeUpdateEvent::TreeUpdateEvent()
    : UpdateAction(Action::UpdateTree) {
}

void TreeUpdateEvent::InitByAdd(SourceUpdateEvent &sourceUpdateEvent, LuaSyntaxTree &t) {
    auto token = t.GetTokenAtOffset(sourceUpdateEvent.Range.StartOffset);
    switch (token.GetTokenKind(t)) {
        case TK_NAME: {
            if (!ContainSpace(sourceUpdateEvent.Text)) {
                auto originRange = token.GetTextRange(t);
                auto start = sourceUpdateEvent.Range.StartOffset - originRange.StartOffset;
                std::string tokenText(token.GetText(t));
                tokenText.insert(start, sourceUpdateEvent.Text);
                if (!LuaLexer::IsReserved(tokenText)) {
                    UpdateAction = Action::OnlyUpdateTokenOffset;
                }
            }
            break;
        }
        case TK_WS: {
            if (OnlySpace(sourceUpdateEvent.Text)) {
                UpdateAction = Action::OnlyUpdateTokenOffset;
            }
            break;
        }
        case TK_STRING: {
            //            auto text = token.GetText(t);
            //            if (text.empty()) {
            //                return;
            //            }
            //            auto del = text.front();
            break;
        }
        case TK_LONG_STRING: {
            break;
        }
        default: {
            break;
        }
    }
}

bool TreeUpdateEvent::ContainSpace(std::string_view source) {
    for (auto c: source) {
        if (c > 0 && std::isspace(c)) {
            return true;
        }
    }
    return false;
}

bool TreeUpdateEvent::OnlySpace(std::string_view source) {
    for (auto c: source) {
        if (c < 0 || !std::isspace(c)) {
            return false;
        }
    }
    return true;
}
