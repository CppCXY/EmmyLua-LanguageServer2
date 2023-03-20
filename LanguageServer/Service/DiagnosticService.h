#pragma once

#include "Service.h"
#include "Lua/SyntaxTree/LuaSyntaxTree.h"

class DiagnosticService : public Service {
public:
    LANGUAGE_SERVICE(DiagnosticService);

    explicit DiagnosticService(LanguageServer *owner);

    std::vector<lsp::Diagnostic> Diagnostic(std::string_view uri);
private:
};

