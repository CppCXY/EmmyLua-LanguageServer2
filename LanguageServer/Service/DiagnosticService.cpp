#include "DiagnosticService.h"
#include "LanguageServer.h"
#include "CodeActionService.h"
#include "ConfigService.h"

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
    auto lineIndex = vFile.GetLineIndex(vfs);
    if(!lineIndex){
        return diagnostics;
    }

    auto &syntaxTree = opSyntaxTree.value();
    if (syntaxTree.HasError()) {
        for(auto& err: syntaxTree.GetErrors()){
            auto& d = diagnostics.emplace_back();
            d.message = err.ErrorMessage;
            d.range = lineIndex->ToLspRange(err.ErrorRange);
        }
    }

    return diagnostics;
}
