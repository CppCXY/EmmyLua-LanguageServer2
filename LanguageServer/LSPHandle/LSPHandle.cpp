#include "LSPHandle.h"
#include "Config/ClientConfig.h"
#include "LanguageServer.h"
#include "Service/CodeActionService.h"
#include "Service/CommandService.h"
#include "Service/ConfigService.h"
#include "Service/DiagnosticService.h"
#include "Util/Url.h"
#include "nlohmann/json.hpp"
#include <fmt/format.h>

using namespace std::placeholders;

LSPHandle::LSPHandle(LanguageServer *server)
    : _server(server) {
    Initialize();
}

LSPHandle::~LSPHandle() {
}

bool LSPHandle::Initialize() {
    JsonProtocol("initialize", &LSPHandle::OnInitialize);
    JsonProtocol("initialized", &LSPHandle::OnInitialized);
    JsonProtocol("textDocument/codeAction", &LSPHandle::OnCodeAction);
    JsonProtocol("textDocument/diagnostic", &LSPHandle::OnTextDocumentDiagnostic);
    JsonProtocol("workspace/diagnostic", &LSPHandle::OnWorkspaceDiagnostic);

    JsonNotification("textDocument/didChange", &LSPHandle::OnDidChange);
    JsonNotification("textDocument/didOpen", &LSPHandle::OnDidOpen);
    JsonNotification("textDocument/didClose", &LSPHandle::OnClose);
    JsonNotification("workspace/executeCommand", &LSPHandle::OnExecuteCommand);
    JsonNotification("workspace/didChangeConfiguration", &LSPHandle::OnWorkspaceDidChangeConfiguration);
    return true;
}

std::shared_ptr<lsp::Serializable> LSPHandle::Dispatch(std::string_view method,
                                                       nlohmann::json params) {
    auto it = _handles.find(method);
    if (it != _handles.end()) {
        return it->second(params);
    }
    return nullptr;
}

std::shared_ptr<lsp::InitializeResult> LSPHandle::OnInitialize(std::shared_ptr<lsp::InitializeParams> params) {
    _server->InitializeService();

    auto result = std::make_shared<lsp::InitializeResult>();

    result->capabilities.textDocumentSync.change = lsp::TextDocumentSyncKind::Incremental;
    result->capabilities.textDocumentSync.openClose = true;

//    result->capabilities.codeActionProvider = true;
//    result->capabilities.executeCommandProvider.commands =
//            _server->GetService<CommandService>()->GetCommands();

    result->capabilities.diagnosticProvider.identifier = "EmmyLua";
    result->capabilities.diagnosticProvider.workspaceDiagnostics = false;
    result->capabilities.diagnosticProvider.interFileDependencies = false;

//    auto &editorConfigFiles = params->initializationOptions.editorConfigFiles;
//    for (auto &configFile: editorConfigFiles) {
//        _server->GetService<ConfigService>()->LoadEditorconfig(configFile.workspace, configFile.path);
//    }

//    std::filesystem::path localePath = params->initializationOptions.localeRoot;
//    localePath /= params->locale + ".json";

//    if (std::filesystem::exists(localePath) && std::filesystem::is_regular_file(localePath)) {
//        _server->GetService<ConfigService>()->LoadLanguageTranslator(localePath.string());
//    }

//    ClientConfig clientConfig;
//    clientConfig.Deserialize(params->initializationOptions.vscodeConfig);
//    _server->GetService<ConfigService>()->UpdateClientConfig(clientConfig);

    return result;
}

std::shared_ptr<lsp::Serializable> LSPHandle::OnInitialized(std::shared_ptr<lsp::Serializable> param) {
    return nullptr;
}

void LSPHandle::OnDidChange(
        std::shared_ptr<lsp::DidChangeTextDocumentParams> params) {
    if (params->contentChanges.size() == 1) {
        auto &content = params->contentChanges.front();
        if (content.range.has_value()) {
            _server->GetVFS().UpdateFile(params->textDocument.uri, content.range.value(),
                                         std::move(content.text));
        } else {
            _server->GetVFS().UpdateFile(params->textDocument.uri,
                                         std::move(content.text));
        }
    } else {
        _server->GetVFS().UpdateFile(params->textDocument.uri, params->contentChanges);
    }
}

void LSPHandle::OnDidOpen(
        std::shared_ptr<lsp::DidOpenTextDocumentParams> params) {
    _server->GetVFS().UpdateFile(params->textDocument.uri, std::move(params->textDocument.text));
}

void LSPHandle::OnClose(
        std::shared_ptr<lsp::DidCloseTextDocumentParams> params) {
    _server->GetVFS().ClearFile(params->textDocument.uri);
}

std::shared_ptr<lsp::CodeActionResult> LSPHandle::OnCodeAction(std::shared_ptr<lsp::CodeActionParams> param) {
    auto codeActionResult = std::make_shared<lsp::CodeActionResult>();

    for (auto &diagnostic: param->context.diagnostics) {
        _server->GetService<CodeActionService>()->Dispatch(diagnostic, param, codeActionResult);
    }
    return codeActionResult;
}

void LSPHandle::OnExecuteCommand(
        std::shared_ptr<lsp::ExecuteCommandParams> params) {
    _server->GetService<CommandService>()->Dispatch(params->command, params);
}

void LSPHandle::OnWorkspaceDidChangeConfiguration(
        std::shared_ptr<lsp::DidChangeConfigurationParams> params) {
    ClientConfig clientConfig;
    clientConfig.Deserialize(params->settings);
    _server->GetService<ConfigService>()->UpdateClientConfig(clientConfig);
    RefreshDiagnostic();
}

std::shared_ptr<lsp::DocumentDiagnosticReport> LSPHandle::OnTextDocumentDiagnostic(
        std::shared_ptr<lsp::DocumentDiagnosticParams> params) {
    auto report = std::make_shared<lsp::DocumentDiagnosticReport>();
    report->kind = lsp::DocumentDiagnosticReportKind::Full;

    auto &vfs = _server->GetVFS();
    auto opFileId = vfs.GetUriDB().Query(params->textDocument.uri);
    if (!opFileId.has_value()) {
        return report;
    }

    auto diagnostics = _server->GetService<DiagnosticService>()->Diagnostic(
            opFileId.value());

    report->items = std::move(diagnostics);
    return report;
}


std::shared_ptr<lsp::WorkspaceDiagnosticReport> LSPHandle::OnWorkspaceDiagnostic(
        std::shared_ptr<lsp::WorkspaceDiagnosticParams> params) {
    return nullptr;
}

void LSPHandle::RefreshDiagnostic() {
    _server->SendRequest("workspace/diagnostic/refresh", nullptr);
}