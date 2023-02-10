#pragma once

#include "Service.h"
#include "LuaParser/Ast/LuaSyntaxTree.h"

class DiagnosticService : public Service {
public:
    LANGUAGE_SERVICE(DiagnosticService);

    explicit DiagnosticService(LanguageServer *owner);

    std::vector<lsp::Diagnostic> Diagnostic(std::size_t fileId);
private:
};

