#include "DiagnosticService.h"
#include "CodeActionService.h"
#include "ConfigService.h"
#include "LanguageServer.h"
#include "VirtualFileSystem.h"

DiagnosticService::DiagnosticService(LanguageServer *owner)
    : Service(owner) {
}

std::vector<lsp::Diagnostic>
DiagnosticService::Diagnostic(std::string_view uri) {
    std::vector<lsp::Diagnostic> diagnostics;

    auto id = _owner->GetService<VirtualFileSystem>()->GetOrCreateId(uri);
    auto document = _owner->GetWorkspace().GetDocument(id);

    auto tree = document->GetSyntaxTree();
    if (!tree) {
        return diagnostics;
    }

    auto &luaSource = tree->GetSource();
    auto &lineIndex = luaSource.GetLineIndex();
    auto &errors = tree->GetSyntaxErrors();
    if (!errors.empty()) {
        for (auto &err: errors) {
            auto &d = diagnostics.emplace_back();
            d.message = err.Message;
            d.range = lineIndex.ToLspRange(err.Range);
        }
    }

    return diagnostics;
}
