#include "DiagnosticService.h"
#include "LanguageServer.h"
#include "CodeActionService.h"
#include "ConfigService.h"

DiagnosticService::DiagnosticService(LanguageServer *owner)
        : Service(owner) {

}

std::vector<lsp::Diagnostic>
DiagnosticService::Diagnostic(std::size_t fileId) {
    return {};
}
