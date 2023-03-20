#include "CodeActionService.h"

#include "CommandService.h"
#include "DiagnosticService.h"
#include "LanguageServer.h"
#include "File/Url.h"

CodeActionService::CodeActionService(LanguageServer *owner)
        : Service(owner) {
}

bool CodeActionService::Initialize() {
//    CodeProtocol(DiagnosticType::Spell, "spell", &CodeActionService::Spell);
//    CodeProtocol(DiagnosticType::Align, "code-align", &CodeActionService::Reformat);
//    CodeProtocol(DiagnosticType::Indent, "code-indent", &CodeActionService::Reformat);
//    CodeProtocol(DiagnosticType::Space, "whitespace", &CodeActionService::Reformat);
//    CodeProtocol(DiagnosticType::StringQuote, "string-quote", &CodeActionService::Reformat);
//    CodeProtocol(DiagnosticType::EndWithNewLine, "end-with-new-line");
//    CodeProtocol(DiagnosticType::NameStyle, "name-style");

    return true;
}

void CodeActionService::Dispatch(lsp::Diagnostic &diagnostic, std::shared_ptr<lsp::CodeActionParams> params,
                                 std::shared_ptr<lsp::CodeActionResult> results) {
    if (diagnostic.code.empty()) {
        return;
    }

    auto it = _handles.find(diagnostic.code);
    if (it != _handles.end()) {
        it->second(diagnostic, params, results);
    }
}


//std::string CodeActionService::GetCode(DiagnosticType code) {
//    auto it = _diagnosticMap.find(code);
//    if (it != _diagnosticMap.end()) {
//        return it->second;
//    }
//
//    return "";
//}

//void CodeActionService::CodeProtocol(DiagnosticType type, std::string_view code, CodeActionHandleMember handle) {
//    _diagnosticMap[type] = code;
//    if (!handle) {
//        return;
//    }
//    _handles[std::string(code)] = [this, handle](auto diagnostic, auto params, auto result) {
//        return (this->*handle)(diagnostic, params, result);
//    };
//}
