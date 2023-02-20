#include "DiagnosticService.h"
#include "CodeActionService.h"
#include "ConfigService.h"
#include "LanguageServer.h"

DiagnosticService::DiagnosticService(LanguageServer *owner)
    : Service(owner) {
}

std::vector<lsp::Diagnostic>
DiagnosticService::Diagnostic(std::size_t fileId) {
    std::vector<lsp::Diagnostic> diagnostics;
    auto &vfs = _owner->GetVFS();
    auto vFile = vfs.GetVirtualFile(fileId);
    auto opSyntaxTree = vFile.GetSyntaxTree(vfs);
    if (!opSyntaxTree.has_value()) {
        return diagnostics;
    }

    auto luaFile = vFile.GetLuaFile(vfs);
    if (!luaFile) {
        return diagnostics;
    }
    auto &lineIndex = luaFile->GetLineIndex();

//    auto &syntaxTree = opSyntaxTree.value();
    auto &errors = luaFile->GetErrors();
    if (!errors.empty()) {
        for (auto &err: errors) {
            auto &d = diagnostics.emplace_back();
            d.message = err.Message;
            d.range = lineIndex.ToLspRange(err.Range);
        }
    }

    return diagnostics;
}
