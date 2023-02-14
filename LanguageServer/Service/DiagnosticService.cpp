#include "DiagnosticService.h"
#include "LanguageServer.h"
#include "CodeActionService.h"
#include "ConfigService.h"

DiagnosticService::DiagnosticService(LanguageServer *owner)
        : Service(owner) {

}

std::vector<lsp::Diagnostic>
DiagnosticService::Diagnostic(std::size_t fileId) {
    auto &vfs = _owner->GetVFS();
    auto opSyntaxTree = vfs.GetVirtualFile(fileId).GetSyntaxTree(vfs);
    if (!opSyntaxTree.has_value()) {
        return {};
    }

    auto &syntaxTree = opSyntaxTree.value();

    std::vector<lsp::Diagnostic> diagnostics;
    if (syntaxTree.HasError()) {
        for(auto& err: syntaxTree.GetErrors()){
            auto& d = diagnostics.emplace_back();
            d.message = err.ErrorMessage;
            d.range = err.ErrorRange;
        }
    }

    return diagnostics;
}
